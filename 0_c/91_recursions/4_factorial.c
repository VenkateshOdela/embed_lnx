
#include <stdio.h>

int fact=1;

int factrl(int a);

int factrl(int a)
{
	if( (a < 0) || (a > 12))
	{
		printf("invalid input \n");
		return 0;
	}
	if(a == 1)
	{
		return fact;
	}

	fact = fact * a;

	return factrl(a-1);
}

int main()
{
	int a = 13;

	int ret = factrl(a);

	printf("%d \n",ret);
}
