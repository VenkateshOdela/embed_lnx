

#include <stdio.h>

/*
 *   i/p 3 2 4 5 1
 *
 *   0. assuming index 0 element has lowest value,
 *   1.then finding smallest element from list,
 *   2.then swaping the smallest number to left;
 *   3. repeating same..
 *
 */


void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

int selection_sort_iterations_count =0;
int bubble_sort_iterations_count =0;


int* selection_sort_ascending(int *a,int n)
{
	int min_value;
	int min_index;

	for(int i=0;i<=n-1;i++)
	{
		//selection_sort_iterations_count++;
		min_value = a[i];
		min_index = i;
		for(int j=i+1;j<=n-1;j++)
		{
			if(a[j]<min_value)
			{
				min_value = a[j];
				min_index = j;
			}
			selection_sort_iterations_count++;
		}
		swap(&a[min_index],&a[i]);
	}
}

int* selection_sort_descending(int *a,int n)
{
	int max_value;
	int max_index;

	for(int i=0;i<=n-1;i++)
	{

		max_value = a[i];
		max_index = i;
		for(int j=i+1;j<=n-1;j++)
		{
			if(a[j]>max_value)
			{
				max_value = a[j];
				max_index = j;
			}
		}
		swap(&a[max_index],&a[i]);
	}
}
/*
 *  1. swap adjacent index elements.
 *  2. starting from right, and pushing a lowest a value to left
 *  3. there by decreasing a iteration count in next loop;
 *
 */
void bubble_sort_ascending(int *a,int n)
{
	int temp;
	int flag; // this flag checking is included to avoid number of iterations for given sorted array


	for(int i = 0; i<n-1; i++)
	{
		//bubble_sort_iterations_count++;
		for(int j=n-1;j >= i+1 ;j--)
		{
            flag =0;
			if(a[j-1] > a[j])
			{
				temp = a[j-1];
				a[j-1] = a[j];
				a[j] = temp;
				flag =1;
			}
			bubble_sort_iterations_count++;
			if(flag == 0)
			{
				break;
			}
		}
	}

}

void insertion_sort(int *arr,int n)
{
	int temp,j;
	for(int i =0; i<= n-1;i++)
	{
		temp=arr[i];
		j = i-1;
		while((j>=0) && arr[j]> temp)
		{
			arr[j+1] = arr[j];
			j--;
		}
        arr[j+1] = temp;
	}


}

int main()
{
	int arr[5] = {3,2,4,5,1};

	//int arr[5] = {1,2,3,4,5};

	int n = sizeof(arr)/sizeof(arr[0]);

	//selection_sort_ascending(arr,n);

	//selection_sort_descending(arr,n);

	//bubble_sort_ascending(arr,n);

	insertion_sort(arr,n);

	int i=0;

	for(i=0;i<5;i++)
	{
		printf("%d \t",arr[i]);
	}

	printf("\n");

	printf("selection_sort_iterations_count = %d \n",selection_sort_iterations_count);

	printf("bubble_sort_iterations_count = %d \n",bubble_sort_iterations_count);


}
