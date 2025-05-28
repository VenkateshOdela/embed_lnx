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

#define NO_OF_DEVS        4

/* structure to hold per device config data */
struct vDev_config_data {
	struct virtual_platform_data pdata;
	dev_t devid;
	char *buffer;
	struct device *dev;
	struct cdev veda_cdev;
};

/* structure to hold driver specific data */
struct vDrv_private_data {
	int total_device;
	dev_t major_base;
	struct class *virtual;
} drv_pdata;

int check_permission(int permission, int access_mode)
{

	if (permission == RDWR)
		return 0;

	/* Read only access */
	if ((permission == RDONLY)
	    && ((access_mode & FMODE_READ) && !(access_mode & FMODE_WRITE)))
		return 0;

	/* Write only access */
	if ((permission == WRONLY)
	    && ((access_mode & FMODE_WRITE) && !(access_mode & FMODE_READ)))
		return 0;

	return -EPERM;
}

int vdev_open(struct inode *inode, struct file *filp)
{
	int minor_no, ret;
	struct vDev_config_data *devconf;
	struct device *dev;

	/* Find out on on which device file open was attempted by user space */
	minor_no = MINOR(inode->i_rdev);

	/* Get device's private data structure */
	devconf =
	    container_of(inode->i_cdev, struct vDev_config_data, veda_cdev);

	dev = devconf->dev;

	/* Supply device private data to other method of the driver */
	filp->private_data = devconf;

	/* Check permission */
	ret = check_permission(devconf->pdata.permission, filp->f_mode);
	if (!ret)
		dev_info(dev, "Open successful\n");
	else
		dev_info(dev, "Open unsuccessful\n");

	return ret;

}

int vdev_release(struct inode *inode, struct file *filp)
{
	return 0;
}

ssize_t vdev_read(struct file *filp, char __user * buff, size_t count,
		  loff_t * f_pos)
{
	struct device *dev;
	int max_size;


	struct vDev_config_data *devconf =
	    (struct vDev_config_data *)filp->private_data;
	dev = devconf->dev;
	max_size = devconf->pdata.size;

	dev_info(dev,"Read requested for %zu bytes \n", count);
	dev_info(dev,"Current file position = %lld\n", *f_pos);

	/* validate request */
	if ((*f_pos + count) > max_size)
		count = max_size - *f_pos;

	if (copy_to_user(buff, devconf->buffer, count))
		return -EFAULT;

	/* Update current file position */
	*f_pos += count;
	dev_info(dev, "Number of bytes successfully read = %zu\n", count);
	dev_info(dev, "Updated file position = %lld\n", *f_pos);

	return count;

}

ssize_t vdev_write(struct file *filp, const char __user * buff, size_t count,
		   loff_t * f_pos)
{
	int max_size;
	struct device *dev;
	struct vDev_config_data *devconf =
	    (struct vDev_config_data *)filp->private_data;
	dev = devconf->dev;
	max_size = devconf->pdata.size;

	dev_info(dev, "Write requested %zu bytes \n", count);
	dev_info(dev, "Current file position = %lld\n", *f_pos);

	if ((*f_pos + count) > max_size)
		count = max_size - *f_pos;

	if (!count)
		return -ENOMEM;

	if (copy_from_user(devconf->buffer, buff, count))
		return -EFAULT;

	/* Update current file position */
	*f_pos += count;
	dev_info(dev, "Number of bytes successfully written = %zu\n", count);
	dev_info(dev, "Updated file position = %lld\n", *f_pos);

	return count;

}

loff_t vdev_lseek(struct file *filp, loff_t offset, int whence)
{
	struct device *dev;
	loff_t temp;
	unsigned int max_size;
	struct vDev_config_data *devconf =
	    (struct vDev_config_data *)filp->private_data;

	dev = devconf->dev;
	max_size = devconf->pdata.size;

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

	dev_info(dev,"New value of the file position = %lld\n", filp->f_pos);
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

static int vdrv_probe(struct platform_device *pdev)
{

	int ret;
	struct device *dev;
	struct vDev_config_data *devconf;
	struct virtual_platform_data *platdata;

	dev = &pdev->dev;

	/* Get platform data */
	platdata = (struct virtual_platform_data *)dev_get_platdata(&pdev->dev);
	if (!platdata) {
		dev_err(dev, "No platform data available\n");
		return -EINVAL;
	}

	devconf = devm_kzalloc(dev, sizeof(*devconf), GFP_KERNEL);
	if (!devconf) {
		dev_err(dev, "Cannot allocate memory\n");
		return -ENOMEM;
	}

	/* Save device private data pointer in platform device structure */
	dev_set_drvdata(&pdev->dev, devconf);

	devconf->pdata.size = platdata->size;
	devconf->pdata.permission = platdata->permission;
	devconf->pdata.serial_number = platdata->serial_number;

	dev_info(dev, "Device size %d\n", devconf->pdata.size);
	dev_info(dev, "Device permission %d\n", devconf->pdata.permission);
	dev_info(dev, "Device serial number %s\n",
		 devconf->pdata.serial_number);

	/* Dynamically allocate memory for the device buffer */
	devconf->buffer = devm_kzalloc(dev, devconf->pdata.size, GFP_KERNEL);
	if (!devconf->buffer) {
		dev_err(dev, "Cannot allocate memory\n");
		return -ENOMEM;
	}

	devconf->devid = drv_pdata.major_base + pdev->id;

	cdev_init(&devconf->veda_cdev, &vdev_ops);
	devconf->veda_cdev.owner = THIS_MODULE;
	ret = cdev_add(&devconf->veda_cdev, devconf->devid, 1);
	if (ret < 0) {
		dev_err(dev, "Cdev add failed\n");
		return ret;
	}

	/* Create device file for the detected platform device */
	devconf->dev =
	    device_create(drv_pdata.virtual, NULL, devconf->devid, NULL,
			  "vDev-%d", pdev->id);

	if (IS_ERR(drv_pdata.virtual)) {
		ret = PTR_ERR(drv_pdata.virtual);
		cdev_del(&devconf->veda_cdev);
		return ret;
	}

	drv_pdata.total_device++;

	dev_info(dev, "Probe successful\n");
	dev_info(dev, "--------------------\n");
	return 0;
}

static int vdrv_remove(struct platform_device *pdev)
{
	struct vDev_config_data *devconf = dev_get_drvdata(&pdev->dev);
	struct device *dev;

	dev = &pdev->dev;
	device_destroy(drv_pdata.virtual, devconf->devid);
	cdev_del(&devconf->veda_cdev);
	drv_pdata.total_device--;

	dev_info(dev, "Device detached\n");
	return 0;
}

struct platform_device_id vDevs_ids[] = {
	{"vDev-Ax", 0},
	{"vDev-Bx", 1},
	{"vDev-Cx", 2},
	{"vDev-Dx", 3},
	{},
};

struct platform_driver vdev_platform_driver = {
	.probe = vdrv_probe,
	.remove = vdrv_remove,
	.id_table = vDevs_ids,
	.driver = {
		   .name = "DRVNAME"}
};

static int __init vdrv_platform_reg(void)
{

	int ret;

	ret =
	    alloc_chrdev_region(&drv_pdata.major_base, 0,
				NO_OF_DEVS, DRVNAME);
	if (ret < 0)
		goto error_out;

	/* Create class and device files </sys/class/...> */
	drv_pdata.virtual = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(drv_pdata.virtual)) {
		ret = PTR_ERR(drv_pdata.virtual);
		goto unregister_allocate_dd;
	}

	ret = platform_driver_register(&vdev_platform_driver);
	if (ret < 0)
		goto class_del;

	pr_info("Platform driver registered\n");

	return 0;

 class_del:
	class_destroy(drv_pdata.virtual);
	unregister_chrdev_region(drv_pdata.major_base, NO_OF_DEVS);
 unregister_allocate_dd:
	unregister_chrdev_region(drv_pdata.major_base, NO_OF_DEVS);
 error_out:
	return ret;

}

static void __exit vdrv_platform_unreg(void)
{
	platform_driver_unregister(&vdev_platform_driver);
	class_destroy(drv_pdata.virtual);
	unregister_chrdev_region(drv_pdata.major_base, NO_OF_DEVS);

	pr_info("Platform driver unregistered\n");
}

module_init(vdrv_platform_reg);
module_exit(vdrv_platform_unreg);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Raghu Bharadwaj<raghu@techveda.org>");
MODULE_DESCRIPTION("Char driver for platform devices");
