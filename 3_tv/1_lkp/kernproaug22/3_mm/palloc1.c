/*
 * __get_free_page() - gets direct address to write data without using other
 *                     two calls alloc_page() and page_address()
 */

#include <linux/module.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/gfp.h>
#include <linux/mm.h>

unsigned long ptr;
int init_module(void)
{
	//ptr = __get_free_page(GFP_KERNEL);
	ptr = __get_free_page(GFP_KERNEL | GFP_DMA); 	
	if(!ptr) 
		return -ENOMEM;
	
	strcpy((char *)ptr, "venkatesh kernel\n");
	pr_info("%s : %p : %s\n", __func__, (char *)ptr, (char*)ptr);	
	free_page(ptr);

	return -EINVAL;
}


void cleanup_module(void)
{
	
}

MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("page allocations");
MODULE_LICENSE("Dual MIT/GPL");
