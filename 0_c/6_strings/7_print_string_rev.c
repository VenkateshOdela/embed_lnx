

#include <stdio.h>
#include <string.h>

void print_string_rev(char str[])
{
	int len = strlen(str);

	printf("len = %d \n",len);

	int i,j,temp;

	for(i=0,j=len-1;i<j;i++,j--)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
	}

	printf("\n");

}

int main()
{
	char str[] = "venkatesh";

	print_string_rev(str);

	printf("%s \n",str);

}

