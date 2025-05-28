
/*
 *  considering 32bit machine;
 *  wordsize = 32/8 = 4bytes;
 *  processor does not read 1 byte in each cpu cycle;
 *  processor read 1 word(4bytes) in each cpu cycle;
 *
 *  if we do #pragma pack(1) adding extra pad bytes is avoided ;
 */

#include <stdio.h>

#pragma pack(1)

struct abc
{
	char a;
	char b;
	int  c;

};

int main()
{
   printf("%ld \n",sizeof(struct abc));
}


/*
 *  expected : 1+1+4 = 6
 *
 *  but output his : 1+1+'0'(pad)+4 = 6 bytes
 */
