
#include <stdio.h>
#include <string.h>

int string_reverse(char *str,int len)
{
	if(len == 0)
	{
		return 0;
	}

    printf("%c",*(str+len-1));

    return string_reverse(str,len-1);
}

int main()
{
	char str[]="venkatesh";
	int len = sizeof(str)/sizeof(str[0]);
	string_reverse(str,len);
}
