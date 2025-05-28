/*
 *   -- alloc_page() - allocating single page
   -- alloc_pages(,) - allocating multiple pages
   -- page_address() - getting page address from allocated page
   -- PAGE_SIZE      - it gives present page_size in RAM
   -- page_to_pfn()  - Conveting page number to frame number
   -- pfn_to_page()  - COnverting frame number to page number
   -- free_page      - freeing the allocated pages
 *
 */

#include <linux/module.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/gfp.h>
#include <linux/printk.h>

struct page *pg;
char *ptr;
int init_module(void)
{
	/*
	 * alloc_page -> single page
	 * alloc_pages -> multi pages : alloc_pages(GFP_KERNEL,1);
	 *                                 1 means 2^1 , 2 means 2^2 , 3 means 2^3
	 */
	// get the page descriptor of the page
	pg = alloc_page(GFP_KERNEL);	
	//pg = alloc_pages(GFP_KERNEL,1);
	if(!pg) 
		return -ENOMEM;
	
	// the page_address goes through page descriptor , returns ptr which is pagenumber + offset(linear address)
	ptr = page_address(pg); 	

	// write operation to linear address
	strcpy(ptr, "Hello World\n");	
	
	pr_info("%s : Page Addr = %p frame size = %ld\n", __func__, pg, PAGE_SIZE);	// frame size/page size = 4096
	pr_info("%s : Page to Frame Number = %ld\n", __func__, page_to_pfn(pg));	
	pr_info("%s : Frame to page Number = %p\n", __func__, pfn_to_page(page_to_pfn(pg)));	
	pr_info("%s : %p : %s\n", __func__, ptr, ptr);	
	free_page((unsigned long)ptr);
	return -EINVAL;
}


void cleanup_module(void)
{
	pr_info("%s : Invoked\n", __func__);
	
}

MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("page allocations");
MODULE_LICENSE("Dual MIT/GPL");

/*
 *[543825.665656] init_module : Page Addr = 0000000097b6e95f frame size = 4096
[543825.665660] init_module : Page to Frame Number = 123981
[543825.665661] init_module : Frame to page Number = 0000000097b6e95f
[543825.665663] init_module : 00000000e8c4316d : Hello World
 *
 *
 */
