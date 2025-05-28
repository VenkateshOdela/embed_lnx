

#include <stdio.h>

void bubble_sort(int a[],int n)
{
	int temp =0;
	for(int i=0;i<=n-1;i++)
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
	int a[5] = {5,4,1,2,3,};

    int n = sizeof(a)/sizeof(a[0]);

    bubble_sort(a,n);

    for(int i=0; i<=n-1; i++)
    {
    	printf("%d ,",a[i]);
    }
}
