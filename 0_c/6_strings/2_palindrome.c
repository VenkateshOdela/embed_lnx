

#include <stdio.h>
#include <string.h>

int main()
{

	char str[] = "radar";
	int len = strlen(str);

	//int len = sizeof(str)/sizeof(str[0]);

    char rev_str[50];

	int i,j;

	for (i=0,j=len-1; i<len; i++,j--)
	{
		rev_str[j] = str[i];
	}

	for(i=0;i<len;i++)
	{
		printf("%c",rev_str[i]);
	}

	if( strcmp(str,rev_str) == 0)
	{
		printf("string is palindrome \n");
	}
	else{
		printf("string is not palindrome \n");
	}

}
