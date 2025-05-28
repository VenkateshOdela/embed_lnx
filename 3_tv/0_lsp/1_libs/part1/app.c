

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int xyz()
{
	printf("test app %d\n", getpid());
	f1();
	return 0;
}
void _start(void)
{
	printf("start %d\n", getpid());
	exit(xyz());
}	
