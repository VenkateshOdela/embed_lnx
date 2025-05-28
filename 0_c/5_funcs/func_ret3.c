/*
 * returning multiple values from a function -using structure variable
 *
 */
#include<stdio.h>

//abc_struct ret_mul_vals(int a, int b);

typedef struct abc{
	int a;
	int b;
	int sum;
	int diff;
}abc_struct;

abc_struct ret_mul_vals(int a, int b)
{
	abc_struct func_obj;

	func_obj.sum = a+b;
	func_obj.diff = a-b;

	return func_obj;
}


int main()
{
	abc_struct obj;
	abc_struct ret_val;

	obj.a = 200;
	obj.b = 100;
	obj.sum =0;
	obj.diff =0;

	ret_val = ret_mul_vals(obj.a,obj.b);

	printf("sum = %d, diff =%d \n",ret_val.sum,ret_val.diff);

	return 0;
}





