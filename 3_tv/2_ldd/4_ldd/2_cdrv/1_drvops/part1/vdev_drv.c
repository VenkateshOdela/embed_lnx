#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include "platform.h"

#define DRVNAME       	"vDev_drv"
#define CLASS_NAME      "VIRTUAL"
#define NO_OF_DEVS        1

struct virtual_platform_data pdata;
dev_t devid;
char *buffer;
struct cdev veda_cdev;
struct class *virtual;

int vdev_open(struct inode *inode, struct file *filp)
{

	pr_info("Open successful\n");
	return 0;
}

int vdev_release(struct inode *inode, struct file *filp)
{
	return 0;
}

ssize_t vdev_read(struct file *filp, char __user * buff, size_t count,
		  loff_t * f_pos)
{

	int max_size;
	max_size = pdata.size;

	pr_info("Read requested for %zu bytes \n", count);
	pr_info("Current file position = %lld\n", *f_pos);

	/* validate request */
	if ((*f_pos + count) > max_size)
		count = max_size - *f_pos;

	if (copy_to_user(buff, buffer, count))
		return -EFAULT;

	/* Update current file position */
	*f_pos += count;
	pr_info("Number of bytes successfully read = %zu\n", count);
	pr_info("Updated file position = %lld\n", *f_pos);

	return count;

}

ssize_t vdev_write(struct file *filp, const char __user * buff, size_t count,
		   loff_t * f_pos)
{
	int max_size;
	max_size = pdata.size;

	pr_info("Write requested %zu bytes \n", count);
	pr_info("Current file position = %lld\n", *f_pos);

	if ((*f_pos + count) > max_size)
		count = max_size - *f_pos;

	if (!count)
		return -ENOMEM;

	if (copy_from_user(buffer, buff, count))
		return -EFAULT;

	/* Update current file position */
	*f_pos += count;
	pr_info("Number of bytes successfully written = %zu\n", count);
	pr_info("Updated file position = %lld\n", *f_pos);

	return count;

}

loff_t vdev_lseek(struct file *filp, loff_t offset, int whence)
{
	loff_t temp;
	unsigned int max_size;
	max_size = pdata.size;

	switch (whence) {
	case SEEK_SET:
		if ((offset > max_size) || (offset < 0))
			return -EINVAL;
		filp->f_pos = offset;
		break;
	case SEEK_CUR:
		temp = filp->f_pos + offset;
		if ((temp > max_size) || (temp < 0))
			return -EINVAL;
		filp->f_pos = temp;
		break;
	case SEEK_END:
		temp = max_size + offset;
		if ((temp > max_size) || (temp < 0))
			return -EINVAL;
		filp->f_pos = temp;
		break;
	default:
		return -EINVAL;
	}

	pr_info("New value of the file position = %lld\n", filp->f_pos);
	return filp->f_pos;

}

struct file_operations vdev_ops = {
	.open = vdev_open,
	.write = vdev_write,
	.read = vdev_read,
	.release = vdev_release,
	.llseek = vdev_lseek,
	.owner = THIS_MODULE
};

/* 
 * step 1: Gather device config
 * step 2: setup resources
 * step 3: register driver as per chosen kernel driver model
 */
static int vdrv_probe(struct platform_device *pdev)
{

	int ret;
	struct device *dev;
	struct virtual_platform_data *platdata;

	/* Get platform data */
	platdata = (struct virtual_platform_data *)dev_get_platdata(&pdev->dev);
	if (!platdata) {
		dev_err(&pdev->dev, "No platform data available\n");
		return -EINVAL;
	}

	pdata.size = platdata->size;
	pdata.permission = platdata->permission;
	pdata.serial_number = platdata->serial_number;

	dev_info(pdev->dev, "Device size %d\n", pdata.size);
	dev_info(pdev->dev, "Device permission %d\n", pdata.permission);
	dev_info(pdev->dev, "Device serial number %s\n", pdata.serial_number);

	/* Dynamically allocate memory for the device buffer */
	buffer = devm_kzalloc(&pdev->dev, pdata.size, GFP_KERNEL);
	if (!buffer) {
		dev_err(&pdev->dev, "Cannot allocate memory\n");
		return -ENOMEM;
	}

	ret = alloc_chrdev_region(&devid, 0, NO_OF_DEVS, DRVNAME);
	if (ret < 0)
		return ret;

	/* Create class and device files </sys/class/...> */
	virtual = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(virtual)) {
		ret = PTR_ERR(virtual);
		return ret;
	}

	cdev_init(&veda_cdev, &vdev_ops);
	veda_cdev.owner = THIS_MODULE;
	ret = cdev_add(&veda_cdev, devid, 1);
	if (ret < 0) {
		dev_err(dev, "Cdev add failed\n");
		return ret;
	}

	/* Create device file for the detected platform device */
	dev = device_create(virtual, NULL, devid, NULL, "vDev-1");

	if (IS_ERR(virtual)) {
		ret = PTR_ERR(virtual);
		cdev_del(&veda_cdev);
		return ret;
	}

	dev_info(dev, "Probe successful\n");
	dev_info(dev, "--------------------\n");
	return 0;
}

static int vdrv_remove(struct platform_device *pdev)
{
	unregister_chrdev_region(devid, NO_OF_DEVS);
	cdev_del(&veda_cdev);
	device_destroy(virtual, devid);
	class_destroy(virtual);

	pr_info("Device removed");
	return 0;
}

struct platform_device_id vDevs_ids[] = {
	{"vDev-Ax", 0},
	{},
};

struct platform_driver vdev_platform_driver = {
	.probe = vdrv_probe,
	.remove = vdrv_remove,
	.id_table = vDevs_ids,
	.driver = {
		   .name = "DRVNAME"}
};

module_platform_driver(vdev_platform_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Raghu Bharadwaj<raghu@techveda.org>");
MODULE_DESCRIPTION("Char driver for platform device");
