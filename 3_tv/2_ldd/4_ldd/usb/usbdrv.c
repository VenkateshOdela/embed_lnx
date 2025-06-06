/*
 * ******************************************************************************
 * This program is part of the source code provided with "Linux Device Drivers"
 * Course offered by Techveda <www.techveda.org>
 *
 * (C) 2020 Raghu Bharadwaj/Techveda
 *
 * Git repository:
 *   https://gitlab.com/techveda/ldd
 * ******************************************************************************
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/usb/ch9.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>

/* Structure to hold all of our device specific stuff */
struct drv_priv {
	struct usb_device *udev;	/* the usb device for this device */
	struct usb_interface *interface;	/* the interface for this device */
	unsigned char *bulk_in_buffer;	/* the buffer to receive data */
	size_t bulk_in_size;	/* the size of the receive buffer */
	__u8 bulk_in_endpointAddr;	/* the address of the bulk in endpoint */
	__u8 bulk_out_endpointAddr;	/* the address of the bulk out endpoint */
	int errors;		/* the last request tanked */
	struct kref kref;
};

#define get_drv_priv(d) container_of(d, struct drv_priv, kref)

static void priv_delete(struct kref *kref)
{
	struct drv_priv *dev = get_drv_priv(kref);
	usb_put_dev(dev->udev);
	kfree(dev->bulk_in_buffer);
	kfree(dev);
}
/*
step 1: verify interfaces & endpoints and grab configuration details.
step 2: Register driver operations as per choosen kernel interface 
*/

static int drv_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	struct drv_priv *dev;
	struct usb_host_interface *iface_desc;
	struct usb_endpoint_descriptor *endpoint;
	size_t buffer_size;
	int i;

	/* allocate memory for our device state and initialize it */
	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev) {
		dev_err(&intf->dev, "Out of memory\n");
		goto error;
	}

	dev->udev = interface_to_usbdev(intf);
	dev->interface = intf;

	/* set up the endpoint information */
	/* use only the first bulk-in and bulk-out endpoints */
	iface_desc = intf->cur_altsetting;
	for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i) {
		endpoint = &iface_desc->endpoint[i].desc;

		if (usb_endpoint_is_bulk_in(endpoint)) {
			/* we found a bulk in endpoint */
			buffer_size = __le16_to_cpu(endpoint->wMaxPacketSize);
			dev->bulk_in_size = buffer_size;
			dev->bulk_in_endpointAddr = endpoint->bEndpointAddress;
			dev->bulk_in_buffer = kmalloc(buffer_size, GFP_KERNEL);
			if (!dev->bulk_in_buffer) {
				dev_err(&intf->dev,
					"Could not allocate bulk_in_buffer\n");
				goto error;
			}
		}

		if (usb_endpoint_is_bulk_out(endpoint)) {
			/* we found a bulk out endpoint */
			dev->bulk_out_endpointAddr = endpoint->bEndpointAddress;
		}
	}
	if (!(dev->bulk_in_endpointAddr && dev->bulk_out_endpointAddr)) {
		dev_err(&intf->dev,
			"Could not find both bulk-in and bulk-out endpoints\n");
		goto error;
	}

	/* save our data pointer in this interface device */
	usb_set_intfdata(intf, dev);

	return 0;
 error:
	if (dev)
		/* this frees allocated memory */
		kref_put(&dev->kref, priv_delete);
	return -ENOMEM;

}

static void drv_disconnect(struct usb_interface *intf)
{
	struct drv_priv *dev;
	dev = usb_get_intfdata(intf);
	usb_set_intfdata(intf, NULL);
	kref_put(&dev->kref, priv_delete);
	dev_info(&intf->dev, "USB Skeleton now disconnected");
}

static struct usb_device_id dev_table[] = {
	{USB_DEVICE(0x0781, 0x5572)},	/* sandisk cruzer */
	{}			/* Null terminator (required) */
};

MODULE_DEVICE_TABLE(usb, dev_table);

static struct usb_driver test_driver = {
	.name = "usb-flashdrive",
	.probe = drv_probe,
	.disconnect = drv_disconnect,
	.id_table = dev_table,
};

module_usb_driver(test_driver);

MODULE_AUTHOR("support@techveda.org");
MODULE_DESCRIPTION("usb storage probe driver");
MODULE_LICENSE("GPL");
