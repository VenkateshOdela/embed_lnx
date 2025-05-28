#include <stdio.h>
#include <libusb-1.0/libusb.h>

#define USB_STORAGE_VID 0x0951 // Vendor ID for Kingston 
#define USB_STORAGE_PID 0x1665 // Product ID for DataTraveler G3

int main() {
    libusb_context *ctx = NULL;
    libusb_device_handle *handle = NULL;
    int r;

    r = libusb_init(&ctx);
    if (r < 0) {
        printf("Error initializing libusb: %s\n", libusb_error_name(r));
        return 1;
    }

    handle = libusb_open_device_with_vid_pid(ctx, USB_STORAGE_VID, USB_STORAGE_PID);
    if (!handle) {
        printf("Error opening device: %s\n", libusb_error_name(r));
        goto exit;
    }

    if (libusb_kernel_driver_active(handle, 0)) {
        r = libusb_detach_kernel_driver(handle, 0);
        if (r < 0) {
            printf("Error detaching kernel driver: %s\n", libusb_error_name(r));
            goto exit;
        }
    }

    r = libusb_claim_interface(handle, 0);
    if (r < 0) {
        printf("Error claiming interface: %s\n", libusb_error_name(r));
        goto exit;
    }

    // Now you can use the handle to communicate with the USB storage device
    // For example, you can read and write data using the libusb_bulk_transfer function
    // ...

    libusb_release_interface(handle, 0);

exit:
    libusb_close(handle);
    libusb_exit(ctx);
    return 0;
}

