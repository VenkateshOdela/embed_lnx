
/*xyz() function is called instead of main()*/


#include <stdio.h>
#include <stdlib.h>

int xyz()
{
	printf("%s: programs main(xyz()) routine\n",__func__);
	return 0;
}

void _start(void)
{
	printf("%s: Programs entry routine\n",__func__);
	xyz();
	exit(0);
}


/*
 *
 * _start()
 * 	- libc_start_main()
 * 	     - _init()- program constructor- init()
 * 		- main()
 * 	       _fini() - program destructor -exit()
 */

/*
xyz() function is called instead of main()

user@ubuntu:~/git_embd_linux/embd_linux/tv/lsp/startup$ gcc startup.c 
/tmp/cccZfnHH.o: In function `_start':
startup.c:(.text+0x23): multiple definition of `_start'
/usr/lib/gcc/x86_64-linux-gnu/7/../../../x86_64-linux-gnu/Scrt1.o:(.text+0x0): first defined here
/usr/lib/gcc/x86_64-linux-gnu/7/../../../x86_64-linux-gnu/Scrt1.o: In function `_start':
(.text+0x20): undefined reference to `main'
collect2: error: ld returned 1 exit status
[1]+  Done                    gedit startup.c */


/* to avoid above error use -nostartfiles 
gcc -nostartfiles startup.c
*/




