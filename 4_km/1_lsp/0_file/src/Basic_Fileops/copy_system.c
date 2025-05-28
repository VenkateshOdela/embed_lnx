//#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
    char c; 
    int in, out;
   // getchar();
    in = open("abc", O_RDWR);
    if (in < 0 )
    {
    write (1,"NO SUCH FILE OR DIRECTORY\n",28);
    exit(0);
    }	
     getchar();
	dup2(in,1);
     getchar();
	close(in);
	printf("Hello World\n");
    exit(0);
}

