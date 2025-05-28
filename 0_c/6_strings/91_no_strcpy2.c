
/*
 *  copy string from source to destination;
 *
 *
 */


#include <stdio.h>
#include <string.h>

void my_strcpy(char *des_str, char *src_str)
{
	int i;

	for(i=0;src_str[i]!=0;i++)
	{
		des_str[i] = src_str[i];
	}

	des_str[i] ='\0';

}

int main()
{
	char src_str[] = "venkatesh";
	char des_str[50];

	my_strcpy(des_str,src_str);

	printf("%s",des_str);
}
