/*
 * Returning Multiple Values from a a function - by passing variable address
 * as a parameter
 *
 * 1.we can pass multiple address of a variable as input parameter to a function
 *   inside function body, we can store return values as at the passed memory locations
 *
 * Note: a function can return only value to it's calling function,as per function syntax;
 *
 */

#include <stdio.h>

void ret_mul_vals(int a,int b,int *, int *, int *);

int main()
{
	int a = 10, b=5, add, sub, mul;

	ret_mul_vals(a,b,&add,&sub,&mul);

	printf("add = %d,sub=%d,mul=%d \n",add,sub,mul);

	return 0;
}

void ret_mul_vals(int a, int b ,int *add, int *sub, int *mul)
{
	*add = a+b;
	*sub = a-b;
	*mul = a*b;
}

