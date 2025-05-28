#include <stdio.h>

void func(int a)
{
	int b;

	printf("a address : %p \n", &a);
	printf("b address : %p \n",&b);

	if (&a < &b)
	{
		printf("stack growing upward.. \n");
	}
	else{
		printf("stack growing downwards..\n");
	}
}


int main()
{
	int a;
	func(a);
}
