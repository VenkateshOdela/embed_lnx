/*
 * module parameters :
 *
 * passing arguments to kernel module
 *
 * same like command line arguments
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>


static int base_port[5];	/* array of integers, minimum 1, max. 5 */
static int irq;			/* single integer */

static char *name = 0;		/* string name, passed as a char pointer */


module_param_array (base_port, int, NULL, 0);
MODULE_PARM_DESC (base_port,"Int. base_port=v1,v2,... Min 1, Max 5 values"); //comment macros helpful to pass parameters

// 0644 : 3rd argument to appear these parameter details in /sys/module/param/parameters
module_param (irq, int, 0644);
MODULE_PARM_DESC (irq, "Integer");

module_param (name, charp, 0644);
MODULE_PARM_DESC (name, "String. Device Driver Name");


int init_module (void)
{
	int i;
	pr_info ("IRQ=%d, Device Name=%s\n", irq, name);
	for (i = 0; i < 5; ++i)
		pr_info ("Base Port %d = %x\n", i + 1, base_port[i]);
	return 0;
}

void cleanup_module (void)
{
        pr_info ("IRQ=%d, Device Name=%s", irq, name);
	pr_info ("Unloading");
}


MODULE_AUTHOR ("raghu@techveda.org");
MODULE_DESCRIPTION ("LKD_CW: module params");
MODULE_LICENSE ("Dual MIT/GPL");

/*
 * passing no parameters :
 *
 * $sudo insmod param.ko
 *
 * [466707.373971] IRQ=0, Device Name=(null)
[466707.373975] Base Port 1 = 0
[466707.373976] Base Port 2 = 0
[466707.373976] Base Port 3 = 0
[466707.373977] Base Port 4 = 0
[466707.373978] Base Port 5 = 0
 *
 * passing arguments
 * $ sudo insmod param.ko name=venkatesh irq=10 base_port=10,20,30,40,50
 *
 *  $ dmesg
 *  [466964.967285] IRQ=10, Device Name=venkatesh
[466964.967287] Base Port 1 = a
[466964.967288] Base Port 2 = 14
[466964.967288] Base Port 3 = 1e
[466964.967288] Base Port 4 = 28
[466964.967289] Base Port 5 = 32
 *
 *
 */
