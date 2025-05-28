#include <stdio.h>

int swap_bytes_in_word(unsigned int);

int main()
{
	unsigned int a=0xABCD;
	unsigned int b;

	printf("%x\n",a);
	b = swap_bytes_in_word(a);

	printf("%x\n",b);
}

int swap_bytes_in_word(unsigned int a)
{
	return ( ((a & 0x00FF)<<8 ) | ((a & 0xFF00)>>8));
}
