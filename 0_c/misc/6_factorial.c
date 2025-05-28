#include <stdio.h>

int fact(int);

int factorial=1;

int fact(int n)
{
	if(n==1)
	{
		return factorial;
	}

	factorial = factorial*n;

	fact(n-1);
}
/*
int fact(int n)
{
	int product=1, i=1;

	while(i<=n)
	{
		product = product * i;
		i++;
	}
	return product;
}*/

int main()
{
	int  n =4;
    printf("%d \n",fact(n));
}
