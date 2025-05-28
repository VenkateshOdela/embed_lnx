
/*
 *  two dimensional array - Dynamic memory allocation using double pointers.
 *   int array[2][3];
 *    2 rows
 *    3 columns
 *    1 2 3
 *    4 5 6
 */

#include <stdio.h>
#include <stdlib.h>

#define rows 2
#define columns 7

int main()
{
	int **d_ptr;
	int i,j;
	int number =1;

	d_ptr = malloc(rows*sizeof(int *));

	for(i=0;i<rows; i++)
	{
	   d_ptr[i] = malloc(columns*sizeof(int));
	}

	for(i=0;i<rows;i++)
	{
		for(j=0;j<columns;j++)
		{
			d_ptr[i][j]=number;
			printf("d_ptr[%d][%d]=%d \n",i,j,d_ptr[i][j]);
			number++;
		}
	}

	return 0;

}
