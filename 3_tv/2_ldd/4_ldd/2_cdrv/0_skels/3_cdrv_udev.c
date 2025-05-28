/*
 *  class_create()
 *  device_create()
 *
 *  /dev/vDevnode is created with device_create dynamically.
 *
 *  $ /sys/class/vDev/vDevnode:
 *   dev power subsystem uevent
 *  $ cat u event
 *    MAJOR=507
 *    MINOR=0
 *    DEVNAME=vDevnode
 *
 *   with help of uevent file devfile is created with DEVNAME
 *
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define DRV_NAME "cdrv_udev"
#define COUNT 1
#define SUCCESS 0

/* Grouping in one structure for easy tracking*/
/* these are called driver private data structure */
static struct cdrv_prv {
	struct cdev *veda_cdev;
	dev_t mydev;
	struct device *dev;
	struct class *vclass;
}ctx;

static int cdrv_open(struct inode *inode, struct file *file)
{
	dev_info(ctx.dev, "%s() invoked\n",__func__);
	return SUCCESS;
}

static int cdrv_release(struct inode *inode, struct file *file)
{
	dev_info(ctx.dev, "%s() invoked \n", __func__);
	return SUCCESS;
}

static ssize_t cdrv_write(struct file *file, const char __user
			  * buf, size_t lbuf, loff_t * offset)
{
	dev_info(ctx.dev, "%s() invoked:Rec'vd data of len = %ld\n", __func__,
		lbuf);
	return lbuf;
}

static ssize_t cdrv_read(struct file *file, char __user * buf,
			 size_t count, loff_t * off)
{
	dev_info(ctx.dev, "%s() invoked.\n"
		"request to read %zu bytes\n", __func__, count);
	return count;
}

static struct file_operations cdrv_fops = {
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
	alloc_chrdev_region(&(ctx.mydev), baseminor, COUNT, DRV_NAME);

	/* Allocate cdev instance */
	ctx.veda_cdev = cdev_alloc();

	/* initialize cdev with fops object */
	cdev_init(ctx.veda_cdev, &cdrv_fops);

	/* register cdev */
	ret = cdev_add(ctx.veda_cdev, ctx.mydev, COUNT);
	if (ret < 0) {
		pr_err("Error registering device driver");
		return ret;
	}

  ctx.vclass = class_create(THIS_MODULE, "vDev");
    	if(!ctx.vclass){
        	unregister_chrdev_region(ctx.mydev, COUNT);
        	return PTR_ERR(ctx.vclass);
    	}

  ctx.dev = device_create(ctx.vclass, NULL, ctx.mydev, NULL, "vDevnode");
      if(!ctx.dev){
       		class_destroy(ctx.vclass);
       		unregister_chrdev_region(ctx.mydev, COUNT);
       		return PTR_ERR(ctx.dev);
    }
	dev_info(ctx.dev,"Driver Registered %s\n", DRV_NAME);
	return SUCCESS;
}

static __exit void cdrv_remove(void)
{
	dev_info(ctx.dev,"%s: Device detached\n", __func__);

	/* remove cdev */
	cdev_del(ctx.veda_cdev);

	/* free major/minor no's used */
	unregister_chrdev_region(ctx.mydev, COUNT);

	device_destroy(ctx.vclass, ctx.mydev);
	class_destroy(ctx.vclass);
}

module_init(cdrv_probe);
module_exit(cdrv_remove);


MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("char driver skeleton");
MODULE_LICENSE("Dual MIT/GPL");
