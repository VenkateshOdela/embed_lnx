#include <stdio.h>

void display_bits(int n);

void count_set_bits(int n);

int main()
{
	int n;

	printf("enter an integer :");
	scanf("%d",&n);

	printf("\n");

	display_bits(n);
	count_set_bits(n);

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

void count_set_bits(int n)
{
	int count = 0;
	int mask=0;
	while(n!=0)
	{
		mask = 0x01;
		count = (n&mask) + count;
		n= n >> 1;
	}
	printf("number of set bits: %d \n",count);
}
