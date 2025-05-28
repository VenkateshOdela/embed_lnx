/*
 *
 *
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

extern int get_counter (void);

int init_module (void)
{
	int a;
	a = get_counter();
	printk("counter set to %d\n", a);
	return 0;
}

void cleanup_module (void)
{
}

MODULE_AUTHOR ("raghu@techveda.org");
MODULE_DESCRIPTION ("LKD_CW : accessing exported symbols");
MODULE_LICENSE ("proprietory");
