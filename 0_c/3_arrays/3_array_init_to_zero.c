
/*
 *   initializing total array to zero;
 *
 *   char array[ARRAY_SIZE] = {0}; // is valid;
 *
 *   //obj.array[ARRAY_SIZE] = {0}; // is invalid
 *
 */

#include <stdio.h>
#include <string.h>

#define ARRAY_SIZE 8

typedef struct struct_name
{
	char array[ARRAY_SIZE];
}array_struct;

int main()
{
	 array_struct obj;
     //char array[ARRAY_SIZE] = {0};
	 array_struct *pobj= &obj;
     int i;
/*
     for(i = 0; i<ARRAY_SIZE; i++)
     {
    	 printf("array[%d]=%d \n",i,array[i]);
     }
*/
     obj.array[1] = 'a';// this declaration is error;
     printf("array[1] = %c\n", pobj->array[1]);
     //memset(obj.array, 0, sizeof(obj.array));

     printf("\n");
/*
     for(i = 0; i<ARRAY_SIZE; i++)
	  {
		 printf("obj.array[%d]=%d \n",i,obj.array[i]);
	  }
*/

}
