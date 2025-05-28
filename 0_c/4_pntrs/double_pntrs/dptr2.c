#include <stdio.h>

int main()
{
	int a =10;

	//int *ptr = &a;
	int *ptr;
	ptr = &a;

	printf("%p \n",ptr);
	//int **dptr = &ptr;
	int **dptr;
    dptr = &ptr; // double pointer holds the address of other pointer variable only
    //dptr = &a; //invalid

	printf("%d \n",*ptr);
	printf("%d \n",**dptr);

	printf("%p \n",*dptr);
}


/*
 *  10
 *  10
 */
