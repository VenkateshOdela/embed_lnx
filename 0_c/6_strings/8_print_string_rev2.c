#include <stdio.h>
#include <string.h>

int main()
{
	char str[] = "venkatesh";

	int i;

	int len = strlen(str);

	for(i=len;i>=0;i--)
	{
		printf("%c",str[i]);
	}
	printf("\n");
}
