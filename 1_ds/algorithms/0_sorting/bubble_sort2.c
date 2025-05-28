

/* bubble sort */

#include <stdio.h>

void bubblesort(int *a,int n)
{
	int temp;
	for(int i= 0; i<n-1;i++)
	{
		for(int j=n-1;j>=i+1;j--)
		{
			if(a[j-1] > a[j])
			{
				temp = a[j];
				a[j] = a[j-1];
				a[j-1] = temp;
			}

		}
	}
}

int main()
{


	int a[10] = {2,4,7,5,3,7,8,5,34,1};

	int n = sizeof(a)/sizeof(a[0]);

	bubblesort(a,n);

	for(int i =0; i<10;i++)
	{
		printf("%d ",a[i]);
	}
}
