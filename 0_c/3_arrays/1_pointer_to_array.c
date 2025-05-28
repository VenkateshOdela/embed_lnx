#include <stdio.h>

int main()
{
	int arr[5] = {1,2,3,4,5};

	int (*ptr)[5] = &arr;

	//printf("%d\t",(*ptr[0]));
	//printf("%d\t",(*ptr[1]));
	//printf("%d\t",(*ptr[2]));
	//printf("%d\t",(*ptr[3]));
	printf("%d\t",(*ptr[4]));
}
