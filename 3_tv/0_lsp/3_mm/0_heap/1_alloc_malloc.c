/*
 *  sbrk will already at top of heap.
 *
 *  then allocation of malloc will not succeed;
 *
 *
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


void *curr_brk, *old_brk, *new_brk;

int main(int argc, char *argv[])
{
	int ret;
	void *ptr, *ptr2;

	printf("[%d]:grab start of heap\n",getpid());
	getchar();	
	/* grab current program break address */
	curr_brk = sbrk(0);
	old_brk = curr_brk;

	printf("\n %p \n", curr_brk);
	getchar();

	/* increase program brk by 100 bytes */
	ptr = (void *) malloc(100);
	if (!ptr){
		fprintf(stderr,"malloc(100) failed\n");
		exit(1);
	}

	/* verify change */
	new_brk = sbrk(0);
	printf("\n %p \n", new_brk);
	getchar();

	/* increase prgram brk by 500 bytes  */
	ptr2 = (void *) malloc(200);
	if (!ptr2){
		fprintf(stderr,"malloc(500) failed\n");
		exit(1);
	}

	/* verify change */
	new_brk = sbrk(0);
	printf("\n %p \n", new_brk);

	getchar();

	/* release 2nd chunck */
	free(ptr2);
	curr_brk= sbrk(0);
	printf("\n %p \n",curr_brk);
	getchar();

	/* release 1st chunck */
	free(ptr);
	curr_brk= sbrk(0);
	printf("\n %p \n",curr_brk);
	return 0;
}
