
/*
 *
 *  bubble sort ascending and descending .....
 */



#include <stdio.h>

int cnt=0;


int main()
{

	int a[5] = { 3,2,1,4,5};

	int i,j;

	int temp;

	for(i=0;i<5;i++)
	{
		//cnt++;
		for(j=0;j<4;j++)
		{
			if(a[j] < a[j+1])
			{
				temp = a[j+1];
				a[j+1] = a[j];
				a[j] = temp;
			}
			cnt++;
		}
	}

	for(i=0;i<5;i++)
	{
		printf("%d \t",a[i]);
	}

     printf("\n");

     printf("%d \n",cnt);

}
