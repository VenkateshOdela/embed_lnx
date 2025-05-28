#include <stdio.h>

int power =1;

int pow_func(int a, int b, int c);

int pow_func(int a, int b, int c)
{
	if(b == 0)
	{
		return power%c;
	}
	power = power * a;

	return pow_func(a,b-1,c);
}

int main()
{
    int a=2;
    int b=4;
    int c=3;
    int ret;

    ret = pow_func(a,b,c);

    printf("%d \n", ret);
}
