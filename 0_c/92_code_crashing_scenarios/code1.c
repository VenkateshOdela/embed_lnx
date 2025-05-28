/*
 *  crash type : segmentation fault;
 *
 *  reading uninitlized ptr may crash the code;
 *
 */


#include <stdio.h>

int main()
{
	int a =10;
	//int *ptr = &a; // intialized pointer with address of a
	int *ptr;  // this is uninitailzed pointer

	printf("%d \n",*ptr); //deferencing the uninitilized ptr will crash the code;

}
