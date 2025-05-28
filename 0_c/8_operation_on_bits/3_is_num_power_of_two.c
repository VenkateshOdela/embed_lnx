/* if number is power of two */
/* set bits are one one */

#include<stdio.h>

void display_bits(int n);
void is_num_power_of_two(int n);

int main()
{
	int n;

	printf("enter a number :");

	scanf("%d",&n);

	printf("\n");

	display_bits(n);

	is_num_power_of_two(n);


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

void is_num_power_of_two(int n)
{
	int count=0;
	int i,mask;
	for(i=0; i<32; i++)
	{
	    mask = 1<<i;

	    if((n&mask) == 0)
	    {

	    }
	    else{
	    	count = count+1;
	    }

	    if(count > 2 )
	    {
	    	printf("number is not power of 2\n");
	    	return;
	    }

	}

	printf("count :%d\n",count);
	printf("number is power of 2 \n");

}
