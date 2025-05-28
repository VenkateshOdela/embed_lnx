/*
 *
 *  the function minicore() check is memory physically mapped or not;
 *  getting virtual address is not meant physically memory assigned;
 *
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>

int main()
{
	int ret;
	void *ptr;
	unsigned char mincore_vec[5];
	size_t page_size;
	size_t size;
	size_t page_index;

	/*get arch specific page size */
	page_size = getpagesize();
	size = page_size * 5;

	/* Allocate 20k buffer(mmap region) */
	posix_memalign(&ptr, page_size, size);

	/* fill buffer with 0's */
	memset(ptr,0,size);

	/* verify physical memory map */
	ret = mincore(ptr, size, mincore_vec);
	if(ret<0)
		perror("mincore");

	/* print results */
	for (page_index = 0; page_index < 5 ; page_index++) {
		if (mincore_vec[page_index] & 1) 
			printf("page %lu active\n", (unsigned long)page_index);
		else
			printf("page %lu not active\n", (unsigned long)page_index);
	}
	/* release buffer */
	munmap(ptr, size);
	return 0;
}

/*
 * Is a Page in Physical Memory?
	For debugging and diagnostic purposes, Linux provides the mincore() function, which
	can be used to determine whether a given range of memory is in physical memory or
	swapped out to disk:

Chapter 9: Memory Management
www.it-ebooks.info
	#include <unistd.h>
	#include <sys/mman.h>

	int mincore (void *start,
	size_t length,
	unsigned char *vec);
A call to mincore() provides a vector delineating which pages of a mapping are in
physical memory at the time of the system call. The call returns the vector via vec and
describes the pages starting at start (which must be page-aligned) and extending for
length bytes (which need not be page-aligned). Each byte in vec corresponds to one
page in the range provided, starting with the first byte describing the first page and
moving linearly forward. Consequently, vec must be at least large enough to contain
(length − 1 + page size) / page size bytes. The lowest-order bit in each byte is
1 if the page is resident in physical memory and 0 if it is not. The other bits are currently
undefined and reserved for future use.
On success, the call returns 0 . On failure, it returns −1 and sets errno to one of the
following:
EAGAIN
Insufficient kernel resources are available to carry out the request.
EFAULT
The parameter vec points at an invalid address.
EINVAL
The parameter start is not aligned to a page boundary.
ENOMEM
[address,address+1) contains memory that is not part of a file-based mapping.
Currently, this system call works properly only for file-based mappings created with
MAP_SHARED . This greatly limits the call’s use.
 */
