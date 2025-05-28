#include <stdio.h>

void func(int **dptr)
{
	int *temp = *dptr;
	*temp = 10;
    dptr = NULL;
}

int main()
{
	int a =5;
	int *ptr = &a;
	func(&ptr);

	printf("%d \n",a);

}
