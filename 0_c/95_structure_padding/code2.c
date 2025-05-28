
/*
 *  considering 32bit machine;
 *  wordsize = 32/8 = 4bytes;
 *  processor does not read 1 byte in each cpu cycle;
 *  processor read 1 word(4 bytes) in each cpu cycle;
 */
#include <stdio.h>


struct abc
{
	int a;
	char b;
	int  c;

};

int main()
{
   printf("%ld \n",sizeof(struct abc));
}


/*
 *  expected : 4+1+4 = 9
 *
 *  but ouput his : 4+1+'3'(pad)+4 = 12 bytes
 */
