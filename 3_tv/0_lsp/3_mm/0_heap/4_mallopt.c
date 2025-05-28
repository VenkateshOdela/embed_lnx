/*
 * customizing the allocations with mallopt()
 *
 *
 *
 *
 * M_TOP_PAD
	The amount of padding (in bytes) used when adjusting the size of the data segment.
	Whenever glibc uses brk() to increase the size of the data segment, it can ask for
	more memory than needed in the hopes of alleviating the need for an additional
	brk() call in the near future. Likewise, whenever glibc shrinks the size of the data
	segment, it can keep extra memory, giving back a little less than it would otherwise.
	These extra bytes are the padding. A value of 0 disables all use of padding.


  M_MMAP_THRESHOLD
	The threshold (measured in bytes) over which an allocation request will be satisfied
	via an anonymous mapping instead of the data segment. Note that allocations
	smaller than this threshold may also be satisfied via anonymous mappings at the
	system’s discretion. A value of 0 enables the use of anonymous mappings for all
	allocations, effectively disabling use of the data segment for dynamic memory
	allocations.


 M_TRIM_THRESHOLD
	The minimum amount of free memory (in bytes) at the top of the data segment
	before glibc invokes sbrk() to return memory to the kernel.
 *
 */


#include <stdio.h>
#include <string.h>
#include <malloc.h>

int main()
{
	int i =100;
	void *p, *p1, *p2;


	mallopt(M_TOP_PAD, 0); // padding to zero means, no 135k allocation, what ever asked allocate
	mallopt(M_MMAP_THRESHOLD, 4096);	// if it requested memory more than 4K , then goto mmap segment.
//	malloc_stats();

	p = (void *)malloc(4096 * 2);  // it will go to mmap
	malloc_stats();
	getchar();

	free(p);
	malloc_stats();
	getchar();

	p1 = (void *)malloc(1024);  //Heap from 1K, but memory allocations are in mulitples of page size , therefore 4K
	malloc_stats();
	getchar();

	free(p1);
	malloc_stats();
	getchar();


	p2 = (void *)malloc(1024);
	malloc_stats();
	getchar();

	free(p2);
	malloc_stats();
	getchar();

	return 0;
}
