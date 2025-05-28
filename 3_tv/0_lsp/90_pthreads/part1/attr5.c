/*
 *
 *
 *
 */

#define _GNU_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>


void *tStart(void *arg)
{
	void *getStkAddr;
	size_t getStkSize;

	pthread_attr_t attr;
	pthread_getattr_np(pthread_self(), &attr);
	pthread_attr_getstack(&attr, &getStkAddr, &getStkSize);
	printf("%s:Stack starts @ add :%p of size :%d\n", __func__,
	       getStkAddr, (int)getStkSize);

	pthread_exit(NULL);
}

int main()
{
	pthread_t t;
	pthread_attr_t attr;

	/* minimum stack per thread = 	16KB
	 * Default = 2MB
	 * Maximum is =8MB;
	 */
	size_t stacksize = 1024 * 16; // 16KB (16*1024 = 16384)
	void *stackaddr;
	int align = getpagesize();

	printf("align = %d \n",align);

	/* allocate memory with page alignment */
	posix_memalign(&stackaddr, align, stacksize);

	printf("%s: memory starts @ add :%p of size :%d\n", __func__, stackaddr,
	       (int)stacksize);

	pthread_attr_init(&attr);
	pthread_attr_setstack(&attr, stackaddr, stacksize);
	pthread_create(&t, &attr, tStart, NULL);
	pthread_exit(NULL);
}
