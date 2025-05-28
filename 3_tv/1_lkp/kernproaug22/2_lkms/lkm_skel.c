/*
 * ******************************************************************************
 * 1.basic structure of kernel module
 *
 */

#include <linux/init.h>
#include <linux/module.h>

#define MODNAME   "lkm_skel"
static int __init lkm_skel_init(void)
{
	printk("%s: inserted\n", MODNAME);
	return 0;		/* success */
}

static void __exit lkm_skel_exit(void)
{
	printk("%s: removed\n", MODNAME);
}

module_init(lkm_skel_init); // initliazer function , insmod
module_exit(lkm_skel_exit); // exit function , rmmod


MODULE_AUTHOR("<insert your name here>");
MODULE_DESCRIPTION("<Description goes here>");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");
