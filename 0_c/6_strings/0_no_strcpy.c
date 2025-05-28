/*
 *  copy strings with out using strcpy
 *
 */
#include <stdio.h>
#include <string.h>

int main()
{
	char str1[20] = "venkatesh";
	char str2[20];

	int i;

	printf("str1 :%ld\n",strlen(str1));

	for (i=0;str1[i]!= '\0';i++)
	{
		str2[i] = str1[i];
	}
	str2[i] = '\0';

	printf("%s\n",str2);
}
