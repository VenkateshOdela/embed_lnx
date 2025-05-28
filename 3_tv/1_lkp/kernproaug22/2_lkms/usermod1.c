/*
 * ******************************************************************************
 *
 * __symbol_get
 *
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

int (*getcount)(void); //function pointer

int init_module (void)
{
	int ret = 0;
	getcount =__symbol_get ("get_counter");
	if (getcount)
	  {
		  ret = (getcount)();
		  pr_info("counter @ %d\n",ret);
		  __symbol_put ("get_counter");
		  return 0;
	  }
	else
		  pr_err ("%s: Symbol not available\n",__func__);
	pr_info(".......\n");
	return 0;
}

void cleanup_module (void)
{
}

MODULE_AUTHOR ("raghu@techveda.org");
MODULE_DESCRIPTION ("LKD_CW : Accessing exported data symbols");
MODULE_LICENSE ("Dual MIT/GPL");
