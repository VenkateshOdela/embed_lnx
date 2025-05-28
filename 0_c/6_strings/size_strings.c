#include <stdio.h>

#define BLOCK_SIZE 512
#define FILE_NAME_TEXT "io-server_status_file: "

int main()
{
	char s0;
	char s1[]="venkatesh";
	char s2[]="odela";

	char temp[BLOCK_SIZE];

	memset(temp, 0x00, sizeof(temp));

	strncpy(temp, FILE_NAME_TEXT, sizeof(temp));

	printf("%d \n",sizeof(s0)); // print size as one byte
	printf("%d \n",sizeof(s1)); // prints size as 10(9+1)bytes with null
	printf("%d \n",sizeof(s2)); // prints size as 6(5+1)bytes with null

	printf("%d \n",sizeof(temp));
}

/*
10
6
*/
