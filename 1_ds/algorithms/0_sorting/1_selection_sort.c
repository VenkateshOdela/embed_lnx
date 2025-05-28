

#include <stdio.h>

void selection_sort(int a[],int n)
{

	int minvalue =0;
	int minindex =0;
	int temp =0;
	for(int i=0;i<=n-1;i++)
	{
		minvalue = a[i];
		minindex = i;

		for(int j=i+1;j<=n-1;j++)
		{
			if(a[j] < minvalue)
			{
				minvalue = a[j];
				minindex = j;
			}
		}

		temp = a[i];
		a[i] = a[minindex];
		a[minindex] = temp;

	}
}

int main()
{
	int a[5] = {5,4,1,2,3,};

    int n = sizeof(a)/sizeof(a[0]);

    selection_sort(a,n);

    for(int i=0; i<=n-1; i++)
    {
    	printf("%d ,",a[i]);
    }
}
