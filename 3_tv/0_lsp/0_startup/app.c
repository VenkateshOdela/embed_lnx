

#include<stdio.h>
#include <unistd.h>

int main()
{
	printf("%s: Welcome to linux world\n",__func__);
	printf("%s: pid %u\n", __func__,  getpid());
	getchar();
	return 0;

}



/*
 *
 * _start()
 * 	- libc_start_main()
 * 	     - _init()- program constructor- init()
 * 		- main()
 * 	       _fini() - program destructor -exit()
 */
