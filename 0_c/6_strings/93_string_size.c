

#include <stdio.h>
#include <string.h>

int main()
{
	char str[] ="ship it!";

	printf("%ld \n",sizeof(str));

	const char str1[] ="ship it!";

	printf("%ld \n",sizeof(str));
}
