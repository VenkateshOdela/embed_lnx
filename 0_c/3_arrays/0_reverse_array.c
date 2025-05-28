
#include <stdio.h>

#define ARRAY_LENGTH 4
int* reverse_array(int *b);

int* reverse_array(int *b)
{
   int i,j;
   int temp;

   for(i=0,j=3; i<j;i++,j--)
   {
	   temp = b[j];
	   b[j] = b[i];
	   b[i] = temp;
   }

   return b;
}




int main()
{

	int a[4];
	int *r;

	printf("before reversing \n");

	for(int i=0;i<4; i++)
	{
		a[i]=i;
		printf("a[%d]=%d \n",i,a[i]);
	}

	r = reverse_array(&a[0]);
	printf("after reversing : \n");

	for(int i=0;i<4; i++)
	{

		printf("a[%d]=%d \n",i,r[i]);
	}

	return 0;
}
