

#include <stdio.h>

void binary_search(int *a,int key, int n)
{
	int low =0;
	int high = n-1;
	int mid;

	while(low<=high)
	{
		mid = (low+high)/2;
		if(a[mid] == key)
		{
			printf("key found =%d",a[mid]);
			return;
		}
		else if(a[mid] > key)
		{
			high = mid-1;


		}
		else if(a[mid] < key)
		{
			low= mid+1;

		}

	}
	printf("key not found");
	return;
}


int main()
{
	int a[11] = {1,2,3,4,5,6,7,8,9,10,11};

	int n = sizeof(a)/sizeof(a[0]);

	int key = 15;

	binary_search(a,key,n);
}
