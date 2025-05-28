#include <stdio.h>

void display_bits(int n);

int main()
{
	int n;

	printf("sizeof int : %ld\n",sizeof(n));
	printf("enter an integer :");
	scanf("%d",&n);

	display_bits(n);

}

void display_bits(int n)
{
	int i;

	for(i=31; i>=0;i--)
	{
		putchar( (n&(1<<i)? '1' : '0') );

		if((i%8) == 0)
		{
			printf(" ");
		}
	}
	printf("\n");
}
