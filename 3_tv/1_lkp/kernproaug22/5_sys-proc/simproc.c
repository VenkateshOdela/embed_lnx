/*
 *
 *  comm bw user space and kernel services
 *
 *  creating simple file in /proc
 *
 *
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>

#define MODNAME "[simpproc]"
#define BUFSIZE  100


static int irq=20;
module_param(irq,int,0660);

static int mode=1;
module_param(mode,int,0660);

static struct proc_dir_entry *ent;

static ssize_t mywrite(struct file *file, const char __user *ubuf,size_t count, loff_t *ppos) 
{
	int num,c,i,m;
	char buf[BUFSIZE];
	if(*ppos > 0 || count > BUFSIZE)
		return -EFAULT;
	if(copy_from_user(buf,ubuf,count))
		return -EFAULT;
	num = sscanf(buf,"%d %d",&i,&m);
	if(num != 2)
		return -EFAULT;
	irq = i; 
	mode = m;
	c = strlen(buf);
	*ppos = c;
	return c;
}

static ssize_t myread(struct file *file, char __user *ubuf,size_t count, loff_t *ppos) 
{
	char buf[BUFSIZE];
	int len=0;
	if(*ppos > 0 || count < BUFSIZE)
		return 0;
	len += sprintf(buf,"irq = %d\n",irq);
	len += sprintf(buf + len,"mode = %d\n",mode);
	
	if(copy_to_user(ubuf,buf,len))
		return -EFAULT;
	*ppos = len;
	return len;
}

static struct proc_ops myops = { 
	.proc_read = myread,
	.proc_write = mywrite,
};

static int simple_init(void)
{
	ent=proc_create("simple",0660,NULL,&myops);
	return 0;
}

static void simple_cleanup(void)
{
	proc_remove(ent);
	pr_info("bye ...\n");
}

module_init(simple_init);
module_exit(simple_cleanup);

MODULE_DESCRIPTION("simple proc file");
MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_LICENSE("Dual BSD/GPL");

/*
 * copy_to_user() - kernel buffer to user buffer
 * copy_from_user()
 *
 *
 */
