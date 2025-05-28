

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#define DRV_NAME "cdrv_misc"
#define COUNT 1
#define SUCCESS 0

struct device *dev;

static int cdrv_open(struct inode *inode, struct file *file)
{
	dev_dbg(dev, "%s() invoked\n",__func__);
	return -EINVAL;
}

static int cdrv_release(struct inode *inode, struct file *file)
{
	dev_dbg(dev, "%s() invoked \n", __func__);
	return SUCCESS;
}

static ssize_t cdrv_write(struct file *file, const char __user
			  * buf, size_t lbuf, loff_t * offset)
{
	dev_dbg(dev, "%s() invoked:Rec'vd data of len = %ld\n", __func__,
		lbuf);
	return lbuf;
}

static ssize_t cdrv_read(struct file *file, char __user * buf,
			 size_t count, loff_t * off)
{
	dev_dbg(dev, "%s() invoked.\n"
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

static struct miscdevice cDrvMisc = {
        .minor = MISC_DYNAMIC_MINOR,    /* allocate misc minor */
        .name = DRV_NAME,
        .fops = &cdrv_fops,         /* drivers fops instance */
};


static __init int cdrv_probe(void)
{
	int ret;

	ret = misc_register(&cDrvMisc);
        if (ret < 0) {
        	dev_err(dev, "%s:failed registering with minor %d",__func__, cDrvMisc.minor);
        	return ret;
        }
	dev = cDrvMisc.this_device;
	dev_dbg(dev, "Driver Registered %s with Minor %d\n", DRV_NAME,cDrvMisc.minor);
	return SUCCESS;
}

static __exit void cdrv_remove(void)
{
	dev_dbg(dev, "%s: Device detached\n", __func__);
	misc_deregister(&cDrvMisc);
}
module_init(cdrv_probe);
module_exit(cdrv_remove);

MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("char driver skeleton");
MODULE_LICENSE("Dual MIT/GPL");
