/*
 * page allocator(alloc_pages) fails when we request morethan 8MB of contiguous memory;
 *
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
	int nop = 1;

	while(1) {
		// 2^11 pages = 2098 pages : (total size 8MB = 2098*frame_size(4k))
		pg = alloc_pages(GFP_KERNEL, nop);	
		if(!pg) 
			return -ENOMEM;
	
		ptr = page_address(pg); 	
		pr_info("%s : order(%d) Page Addr = %p\n", __func__, nop, ptr);	
		free_pages((unsigned long)ptr, nop);
		nop += 1;
    }
	return 0;
}


void cleanup_module(void)
{
	pr_info("%s : Invoked\n", __func__);
	
}

MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("page allocations");
MODULE_LICENSE("Dual MIT/GPL");

/*
 *
 * [49190.658139] pagemax: loading out-of-tree module taints kernel.
[49190.667950] pagemax: module verification failed: signature and/or required key missing - tainting kernel
[49190.719831] init_module : order(1) Page Addr = 00000000aca01a3e
[49190.719836] init_module : order(2) Page Addr = 0000000011bd9cf9
[49190.720198] init_module : order(3) Page Addr = 00000000a7729b5e
[49190.720232] init_module : order(4) Page Addr = 0000000020aff66d
[49190.720246] init_module : order(5) Page Addr = 00000000d751e3f1
[49190.720320] init_module : order(6) Page Addr = 00000000d751e3f1
[49190.720397] init_module : order(7) Page Addr = 00000000d751e3f1
[49190.720580] init_module : order(8) Page Addr = 00000000d751e3f1
[49190.721054] init_module : order(9) Page Addr = 000000001be7cdde
[49190.933273] init_module : order(10) Page Addr = 000000006a25d248
 *
 */

/*
 * Note: page allocator fails when we request morethan 8MB of contiguous memory;
 * this allocpages() about getting contiguous memory;
 *
 * large allocations > 120MB and >200MB are requested by DMA;
 *
 *
 */
