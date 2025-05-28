
/*
 *  considering 32bit machine;
 *  wordsize = 32/8 = 4bytes;
 *  processor does not read 1 byte in each cpu cycle;
 *  processor read 1 word(4bytes)in each cpu cycle;
 */
#include <stdio.h>


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
 *  but ouput his : 1+1+'2'(pad)+4 = 8 bytes
 */
