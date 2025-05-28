

/*
 *  two pointer approach using recursion
 */

#include <stdio.h>
#include <string.h>

int func(char *str, char *start_ptr, char *end_ptr)
{
	//printf(""%p)
	if(start_ptr > end_ptr)
	{
		printf("-");
		return 1;
	}
	if( *start_ptr == *end_ptr)
	{
		printf("..");
		return func(str,++start_ptr, --end_ptr);
	}
	else
	{
		printf("+");
	    return 0;
	}
}

int main()
{

	char str[] = "radar";

    int len = strlen(str);

    char *start_ptr = str;
    char *end_ptr = str+len-1;

    int ret = func(str,start_ptr,end_ptr);

    if(ret)
    {
    	printf("string is palindrome \n");
    }
    else{
    	printf("string is not \n");
    }


}
