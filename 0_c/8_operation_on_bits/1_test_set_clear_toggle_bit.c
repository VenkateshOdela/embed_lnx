#include <stdio.h>

void display_bits(int n);

void test_bit(int n, int bit_position);
void set_bit(int n, int bit_position);
void clear_bit(int n, int bit_position);
void toggle_bit(int n, int bit_position);

int main()
{
   int n;
   int bit_position;
   printf("enter a integer : ");

   scanf("%d",&n);

   printf("\n");

   display_bits(n);

   printf("enter bit position :");

   scanf("%d",&bit_position);

   test_bit(n,bit_position);

   set_bit(n,bit_position);

   clear_bit(n,bit_position);

   toggle_bit(n,bit_position);
}

void display_bits(int n)
{
	int i;
   // putchar('binary pattern of integer: \n ');
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

void test_bit(int n, int bit_position)
{

	int mask = 1<<(bit_position);

	int bit=0;

	if(n&mask)
	{
		bit =1;
	}
	else
	{
		bit =0;
	}

	printf("%d bit position value : %d",bit_position,bit);

	printf("\n");

}

void set_bit(int n, int bit_position)
{
    printf("set_bit \n");
	int mask = 1<<(bit_position);

	int bit=0;

	n = n | mask;

	display_bits(n);

	printf("\n");

}

void clear_bit(int n, int bit_position)
{
	printf("clear bit \n");
	int mask = 1<<(bit_position);

	int bit=0;

	n = n & (~mask);

	display_bits(n);

	printf("\n");

}

void toggle_bit(int n, int bit_position)
{
	printf("toggle bit \n");
	int mask = 1<<(bit_position);

	int bit=0;

	n = n ^ mask;

	display_bits(n);

	printf("\n");

}



