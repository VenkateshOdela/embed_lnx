/*
 * two threads t1 and t2 is created(pthread_create)and both threads have same thread function
 * pthread_exit() function is used to terminate the calling thread;
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

typedef struct {
	int a;
	int b;
} ABC;

static void *tStart(void *arg)
{
	ABC *p = (ABC *) arg;
	printf("\n%s: In start routine of thread with id %u\n", __func__,
	       (unsigned int)pthread_self());
	printf("%s: recv'd paramenter instance with state %d & %d\n", __func__,
	       p->a, p->b);
	getchar();

	pthread_exit(NULL); //The  pthread_exit()  function  terminates  the calling thread
}

int main()
{
	pthread_t t1, t2;
	void *res;
	int ret;

	ABC obj;
	obj.a = 10;
	obj.b = 20;

	ret = pthread_create(&t1, NULL, tStart, &obj);
	if (ret != 0) {
		perror("Pthread_create \n");
		exit(ret);
	}

	ret = pthread_create(&t2, NULL, tStart, &obj);
	if (ret != 0) {
		perror("Pthread_create \n");
		exit(ret);
	}

	printf("%s:Threads initialised, wait for completion\n", __func__);
	pthread_exit(NULL); //The  pthread_exit()  function  terminates  the calling thread

	printf("end of main\n"); // this line is not executed as pthread_exit called before

	return 0;
}
