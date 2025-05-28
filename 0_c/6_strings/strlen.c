/*
 * strlen - calculate the length of a string;
 *
 */

#include <stdio.h>
#include <string.h>

// implicit declaration warning is observed when necessary #include are not added;

int main()
{

	char *s1 ="venkatesh";

	char *ip="192.168.0.1";

	printf("s1:%d \n",strlen(s1));

	printf("ip:%d \n",strlen(ip));

}
