#include <stdio.h>
#include <stdlib.h>

int main()
{
	int *ptr = malloc(10);

	ptr++;

	free(ptr);

}
/*
 *
 * user@ubuntu:~/git_embd_linux/embd_linux/0_c/9_dynmc_prgrmng$ ./a.out
free(): invalid pointer
Aborted (core dumped)
 *
 */

/*
 * #include <stdio.h>
#include <stdlib.h>

int main()
{
	int *ptr = malloc(10);

	ptr++;

	free(ptr);
}
The code you provided has undefined behavior, because you are trying to free a memory address that was not returned by malloc.

When you call malloc(10), it returns a pointer to a block of memory of size 10 bytes, and you assign this pointer to the variable ptr.
However, you then increment the pointer ptr by one, so it no longer points to the beginning of the block of memory that was allocated by malloc.

When you call free(ptr), you are passing a pointer to a memory address that was not returned by malloc.
This can cause undefined behavior, and may result in a segmentation fault or other errors.

To avoid this problem, you should always pass the same pointer that was returned by malloc to free.
In this case, you should use the original value of ptr to call free, like this:





how does free() know how many bytes to remove?

	-	The free() function does not need to know how many bytes to remove from memory,
		because it already knows the size of the block of memory that it needs to free.

	-	When you call malloc() or a related function to allocate memory, the system usually keeps track of the size of the allocated block of memory,
		usually in a data structure known as the memory control block (MCB).

	-	The MCB contains information such as the size of the allocated block, whether the block is currently in use or not, and possibly other metadata.

When you call free(), it uses this information in the MCB to determine the size of the block to free.
The free() function then returns the block of memory to the operating system or to a memory pool managed by the C runtime library,
so that it can be reused for future memory allocations.

It's important to note that free() only needs to know the starting address of the memory block that it needs to free, not its size.
The size of the block is stored separately in the MCB or other metadata associated with the block.

This allows free() to efficiently free memory without requiring any additional arguments to specify the size of the block.
 */
