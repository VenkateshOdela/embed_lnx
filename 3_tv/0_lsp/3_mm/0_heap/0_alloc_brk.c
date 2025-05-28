/*
 *  usage of brk, sbrk:
 *
 *  brk will allocate memory from top of heap.
 *
 *  sbrk will point to top of heap.
 *
 */
#include <unistd.h>
#include <stdio.h>

void *curr_brk, *old_brk, *new_brk;

int main()
{
	int ret;
	int *p;

	printf("[%d]:retrieve heap start\n",getpid());
	getchar();	
	/* grab current program break address */
	curr_brk = sbrk(0);
	old_brk = curr_brk;

	printf("\n %p \n", curr_brk);
//	getchar();

	/*increase program brk by 100 bytes*/
	brk(curr_brk + 100);

	/* verify change */
	new_brk = sbrk(0);
	printf("\n %p \n", new_brk);
	old_brk = new_brk;
//	getchar();

	/*increase program brk by 200 bytes*/
	brk(new_brk + 200);
	getchar();

	/* verify change */
	new_brk = sbrk(0);
	printf("\n %p \n", new_brk);
	getchar();

	/* restore program break to previous address */
	brk(old_brk);
	curr_brk= sbrk(0);
	printf("\n %p \n",curr_brk);
	getchar();
	return 0;
}
