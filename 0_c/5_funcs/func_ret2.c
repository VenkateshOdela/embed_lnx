/*
 * Returning Multiple values from a function - using array;
 *
 */
#include <stdio.h>

int* ret_mul_vals(int *);

int main()
{
	int array[5];

	ret_mul_vals(&array[0]);

	for(int i =0; i<5; i++)
	{
		printf("array[%d]=%d\n",i,array[i]);
	}

}

int* ret_mul_vals(int *array)
{

	for(int i =0; i<5; i++)
	{
		array[i]=i*1000;
	}
	return array;
}
