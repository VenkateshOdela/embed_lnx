/*
 *
 *
 */

#include <linux/module.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

void *ptr;
int init_module(void)
{
	ptr = vmalloc(6000);
	pr_info("Value of ptr (kernel Virtual Address) = %p\n",ptr);
	strcpy(ptr,"data written to page\n");
	return 0;
}

void cleanup_module(void)
{
	vfree(ptr);
	pr_info("Unloading\n");
}

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");

/*
 * virtual address here means not really contiguous.
 *
 * vmalloc() -> vfree()
 *
 * vzalloc()-> zeros initilized memory
 */

/*
 * |Cache Allocator|
      |
   |Slab Allocator|   |Fragment Allocator|  |DMA Mapping|    |CMA|
                       (vmalloc)
    |                   |                      |              |
_____________________________________________________________
|                  Page Allocator                            |
|____________________________________________________________|
                          |
_______________________________________________________________
|                    Buddy System                              |
|______________________________________________________________|
        |                    |                          |
    |ZONE_DMA32|		|ZONE_NORMAL|               |ZONE_DEVICE|


 -- When 16MB 64MB memory allocation required?
    -- page allocator will fail.
    -- slab allocator will fail.

 -- Fragment Allocator
   -- virtually contigouos memory. (here and there , from different fragments)
   -- vmalloc() - v for virtual
     -- gfp flags are not required

 */

/*
 * kvmalloc() - kmalloc + vmalloc;
 *
 * kmalloc suceeds upto <4MB
 *
 * kvmalloc succeeds upto physical ram exhaust;
 *
 *
 *
 */


