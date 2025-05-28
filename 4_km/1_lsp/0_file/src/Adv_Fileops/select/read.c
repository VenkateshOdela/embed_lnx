#include<stdio.h>

int main()
{
	int ret;
	char buff[4096];

	ret = read(0,buff,4096);

	return 0;
}
