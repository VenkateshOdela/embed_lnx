/*
 *
 * Difference : register_chrdev_region() vs alloc_chrdev_region()
 *
 * int register_chrdev_region(dev_t first, unsigned int count,char *name);
 *                        |
 *                        |
 *                        |
 * int alloc_chrdev_region(dev_t *dev, unsigned int firstminor,unsigned int count, char *name);
 *
 *
 *  mknod is done after knowing major number from alloc_chrdev_region()
 *  we are creating /dev/abc for testing in this program , bu this can be avoided by device_create()
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define DRV_NAME "cdrv_dyn"
#define COUNT 1
#define SUCCESS 0

static struct cdev *veda_cdev;
static dev_t mydev;

static int cdrv_open(struct inode *inode, struct file *file)
{
	pr_info("%s() invoked\n", __func__);
	return SUCCESS;
}

static int cdrv_release(struct inode *inode, struct file *file)
{
	pr_info("%s() invoked \n", __func__);
	return SUCCESS;
}

static ssize_t cdrv_write(struct file *file, const char __user
			  * buf, size_t lbuf, loff_t * offset)
{
	pr_info("%s() invoked:Rec'vd data of len = %ld\n", __func__,
		lbuf);
	return lbuf;
}

static ssize_t cdrv_read(struct file *file, char __user * buf,
			 size_t count, loff_t * off)
{
	pr_info("%s() invoked.\n"
		"request to read %zu bytes\n", __func__, count);
	return count;
}

static struct file_operations cdev_fops = {
	.owner = THIS_MODULE,
	.write = cdrv_write,
	.read = cdrv_read,
	.open = cdrv_open,
	.release = cdrv_release
};

static __init int cdrv_probe(void)
{
	int ret;
	int baseminor = 0;
	alloc_chrdev_region(&mydev, baseminor, COUNT, DRV_NAME);

	/* Allocate cdev instance */
	veda_cdev = cdev_alloc();

	/* initialize cdev with fops object */
	cdev_init(veda_cdev, &cdev_fops);

	/* register cdev */
	ret = cdev_add(veda_cdev, mydev, COUNT);
	if (ret < 0) {
		pr_err("Error registering device driver");
		return ret;
	}
	pr_info("Driver Registered %s\n", DRV_NAME);
	return SUCCESS;
}

static __exit void cdrv_remove(void)
{
	/* remove cdev */
	cdev_del(veda_cdev);

	/* free major/minor no's used */
	unregister_chrdev_region(mydev, COUNT);

	pr_info("%s: Device detached\n", __func__);
}
module_init(cdrv_probe);
module_exit(cdrv_remove);

MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("char driver skeleton");
MODULE_LICENSE("Dual MIT/GPL");
