/*
 * Finding endianess:
 *
 *  endianess defines way in which bytes are stored in memory
 *
 *   little endianess: lsb of number is stored in first memory address
 *
 *
 */

#if 1
#include <stdio.h>

int main()
{
	int number = 0x12345678;

	//int *int_ptr = &number;
	//printf("*ptr=%x \n",*int_ptr);

	//char *char_ptr= (char *)&number;

	char *char_ptr= (char *)&number;

	//if(*ptr == 0x78)
	if(char_ptr[0] == 0x78)
	{
		printf("Arch is little endianess \n");
		//printf("ptr[0]=%x \n",ptr[0]);
		printf("ptr[0]=%x \n",char_ptr[0]);
	}
	else
	{
		printf(" Arch is big endianess \n");
	}

	for(int i =0;i<4;i++)
	{
		printf("char_ptr[%d]=%x\n",i,char_ptr[i]);
	}
}
#endif

#if 0
#include <stdio.h>

int main()
{
	int number = 0x12345678;
	//int number = 0x12345678;
	int *ptr= (char *)&number;

	//if(ptr[0] == 0x78)
   if(*ptr == 0x78)
	{
		printf("Arch is little endianess \n");
		//printf("ptr[0]=%x \n",ptr[0]);
		printf("*ptr=%x \n",*ptr);
	}
	else
	{
		printf(" Arch is big endianess \n");
	}
}
#endif
