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

	/* taking char_ptr is most important as we are examining byte by byte*/
	char *char_ptr= (char *)&number; 

	if(char_ptr[0] == 0x78)
	{
		printf("Arch is little endianess \n");
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

/*
@VenkateshOdela ➜ /workspaces/embed_lnx/0_c/misc (main) $ ./a.out
Arch is little endianess 
ptr[0]=78 
char_ptr[0]=78
char_ptr[1]=56
char_ptr[2]=34
char_ptr[3]=12
*/

#if 0
/* wrong code*/
#include <stdio.h>

int main()
{
	int number = 0x12345678;
	//int number = 0x12345678;
    /*below is not good */
	/*
	warning: initialization of ‘int *’ from incompatible pointer type ‘char *’ [-Wincompatible-pointer-types]
	int *ptr= (char *)&number;
	
	 */
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

	for(int i =0;i<4;i++)
	{
		printf("ptr[%d]=%x\n",i,ptr[i]);
	}
	
	/*
	@VenkateshOdela ➜ /workspaces/embed_lnx/0_c/misc (main) $ ./a.out
	Arch is big endianess 
	ptr[0]=12345678
	ptr[1]=1
	ptr[2]=bdbfd968
	ptr[3]=7ffd
	*/


}
#endif
