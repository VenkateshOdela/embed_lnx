/*
 * 1. Getting size of struct page as 64 bytes.(sizeof(struct page))
 * 2. checking number of pages in RAM. get_num_physpages()
 * 3. Allocate memory from RAM using kmalloc()
 *
 *
 *
 */

#include <linux/module.h> 
#include <linux/mm.h>    // for memory management
#include <linux/slab.h>  // for memory management

int init_module(void)
{
	unsigned char *ptr;
	unsigned char *pa;

	pr_info("%s : Invoked\n", __func__);
	pr_info("%s : Size of struct page = %ld\n", __func__, sizeof(struct page)); // it is should be 64 bytes not 4K;
	pr_info("%s : Number of Pages = %ld\n", __func__, get_num_physpages()); // number of pages = size of ram /frame size
	
	ptr = kmalloc(4096, GFP_KERNEL); // this kmalloc goes to one of allocator algorithms high level code
	
	// ptr = page number + offset, is physical page address ? no , we call it linear address
    pr_info("%s : Value of ptr (Linear Address) = %p\n", __func__, ptr);

    //page frame table(contains physical memory), kernel macro _pa(ptr) gets physical address mapped to it;
	// rw operation through this physical address is not possible,
    // because cpu is in virtual address mode
	pa = (unsigned char *) __pa(ptr);	
	
	pr_info("%s : Value of pa = %p\n", __func__, pa);

	strcpy(ptr, "data written to page\n"); //write operation to virtual address
	kfree(ptr); // release memory

	return -EINVAL;
}


void cleanup_module(void)
{
}

MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("Page allocation");
MODULE_LICENSE("Dual MIT/GPL");

/*
 *
 *
 * 1 mmtest.c
  - get the size of struct page
  - get the number of pages
  - we can esitmate the physical ram size = 4K * Number of pages
  - ptr = kmalloc returns address, i.e linear address which contains physical address = page_number + offset
  - is this page number really a physical page number?
  - mmu should convert page to frame
  - finding equivalent physical address
  - __pa(ptr) - kernel macro -> look into 'page frame table'
  - macro helps to get mapped frame number
  - = pa= __pa(ptr) , if you pass address it returns equivalent physical address.
  - can i perform read/write operation pa ? no
  - cpu is now in virtual address mode, any address you give now to cpu treat it as virtual address,
  - you can also allocated physical address, but you can read/write with pa.
  - so strcpy(ptr,"data") is correct;
  - kfree(ptr);
 *
 */

/*
 *
 * [507304.790424] init_module : Invoked
[507304.790427] init_module : Size of struct page = 64
[507304.790429] init_module : Number of Pages = 1048429
[507304.790464] init_module : Value of ptr (Linear Address) = 0000000056c8c852
[507304.790465] init_module : Value of pa = 00000000129db485
 *
 */
