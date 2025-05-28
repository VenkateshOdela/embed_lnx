/*
 * crash type : double free;
 * when we do free() second time with same address,
 * the code will crash;
 *
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
	char *ptr = (char *)malloc(3*sizeof(char));


	 for(int i=0;i<3;i++)
	 {
		 ptr[i] = i+10;
	 }


	 for(int i=0;i<3;i++)
	 {
		 printf("%d ",ptr[i]);
	 }

	 free(ptr); // first time free

	 printf("\n after free \n");

	 for(int i=0;i<3;i++)
	 {
		 printf("%d ",ptr[i]);
	 }

	 free(ptr); // second time freeing same memory

}
