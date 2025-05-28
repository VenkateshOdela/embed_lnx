/*
 * ******************************************************************************
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/jiffies.h>

int rate_limit_init(void)
{
	int count;

	for (count = 0; count < 20; count++) {
		if (printk_ratelimit()) {
			pr_info("Printing %d", count);
		}
		
		else
		    ssleep(10);
		
	}

	return 0;
}

void rate_limit_exit(void)
{
	pr_info("Done");
}

module_init(rate_limit_init);
module_exit(rate_limit_exit);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("raghu@techveda.org");
MODULE_DESCRIPTION("printk rate limit");

