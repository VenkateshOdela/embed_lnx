/*
 * register_chrdev() function usage replacing device number allocation functions
 *                   ans cdev_init() and cdev_add() functions
 *
 *
 * register_chrdev() is wrapper to below functions
 *  	-	1. Device Number Registration :
				-	register_chrdev_region()          	-	unregister_chrdev_region()
				-	alloc_chrdev_region()
		-	2. Make a char driver registration with VFS :
				-	cdev_init()							-	cdev_del()
				-	cdev_add()
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define DRV_NAME "cdrv_reg"
#define COUNT 1
#define SUCCESS 0

static struct drv_prv {
	struct cdev *veda_cdev;
	dev_t mydev;
	struct device *dev;
	struct class *vclass;
	struct device *dev_ret;
}ctx;

static int cdrv_open(struct inode *inode, struct file *file)
{
	 dev_dbg(ctx.dev, "%s() invoked\n"
                " minor # is %d\n",
                __func__, iminor(inode));

	return SUCCESS;
}

static int cdrv_release(struct inode *inode, struct file *file)
{
	dev_dbg(ctx.dev, "%s() invoked \n", __func__);
	return SUCCESS;
}

static ssize_t cdrv_write(struct file *file, const char __user
			  * buf, size_t lbuf, loff_t * offset)
{
	dev_dbg(ctx.dev, "%s() invoked:Rec'vd data of len = %ld\n", __func__,
		lbuf);
	return lbuf;
}

static ssize_t cdrv_read(struct file *file, char __user * buf,
			 size_t count, loff_t * off)
{
	dev_dbg(ctx.dev, "%s() invoked.\n"
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
	int minor = 0;

	ret = register_chrdev(0, DRV_NAME, &cdrv_fops);
        if (ret < 0) {
                pr_err("failed acquiring major no");
                return ret;
        }
        ctx.mydev = MKDEV(ret, minor);

        ctx.vclass = class_create(THIS_MODULE, "vDev");
    	if(!ctx.vclass){
        	return PTR_ERR(ctx.vclass);
    	}

        ctx.dev = device_create(ctx.vclass, NULL, ctx.mydev, NULL, "cdrvreg");
        if(!ctx.dev){
       		 class_destroy(ctx.vclass);
       		 return PTR_ERR(ctx.dev);
    }
	dev_dbg(ctx.dev, "Driver Registered %s\n", DRV_NAME);
	return SUCCESS;
}

static __exit void cdrv_remove(void)
{
	unregister_chrdev(MAJOR(ctx.mydev), DRV_NAME);

	device_destroy(ctx.vclass, ctx.mydev);
	class_destroy(ctx.vclass);

	dev_dbg(ctx.dev, "%s: Device detached\n", __func__);
}
module_init(cdrv_probe);
module_exit(cdrv_remove);

MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("char driver skeleton");
MODULE_LICENSE("Dual MIT/GPL");
