#include <stdio.h>
#include <string.h>

void my_memcpy(void *dest, void *src, int n);

int main()
{
	char str1[20] = "venkatesh";
	char str2[20];

	int arr1[5] = {255555,2,3,4,5};
	int arr2[5];

    printf("%ld \n",strlen(str1));
	my_memcpy(str2,str1,strlen(str1));

	printf("%s \n",str2);

	printf("%ld \n",sizeof(arr1));
	my_memcpy(arr2,arr1,sizeof(arr1));

	for(int i =0; i<5;i++)
	{
		printf("%d ", arr2[i]);
	}
}

void my_memcpy(void *dest, void *src, int n)
{
	char *csrc = (char *)src;
	char *cdest = (char *)dest;

	for(int i =0; i<=n;i++)
	{
		cdest[i] = csrc[i];
	}
}
