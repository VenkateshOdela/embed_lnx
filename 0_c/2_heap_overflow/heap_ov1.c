/*
 * Heap-based Buffer Overflow.
 *
 * A heap overflow condition is a buffer overflow, where the buffer that can be overwritten
 * is allocated in the heap portion of memory, generally meaning that the buffer was allocated
 * using a routine such as malloc().
 *
 * The buffer is allocated heap memory with a fixed size, but there is no guarantee the string in argv[1]
 * will not exceed this size and cause an overflow.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024*136

int main(int argc, char **argv)
{
	//printf("%s \n",argv[1]);

	char *buf;
	buf = (char *)malloc(sizeof(char)*BUFSIZE);

	strcpy(buf, argv[1]);
	//strcpy(buf[1], argv[2]);

	printf("%s \n",buf);
	//printf("%s \n",buf[1]);
}

/*
While buffer overflow examples can be rather complex, it is possible to have very simple, yet still exploitable, heap-based buffer overflows:

(bad code)
Example Language: C
#define BUFSIZE 256
int main(int argc, char **argv) {
char *buf;
buf = (char *)malloc(sizeof(char)*BUFSIZE);
strcpy(buf, argv[1]);
}
The buffer is allocated heap memory with a fixed size, but there is no guarantee the string in argv[1] will not exceed this size and cause an overflow.
*/
