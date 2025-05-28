#include <stdio.h>

int swap_nibbles_in_byte(unsigned char);

int main()
{
	unsigned char a=0xAB;
	unsigned char b;

	printf("%x\n",a);
	b = swap_nibbles_in_byte(a);

	printf("%x",b);
}

int swap_nibbles_in_byte(unsigned char a)
{
	return ( ((a & 0x0F)<<4 ) | ((a & 0xF0)>>4));
}
