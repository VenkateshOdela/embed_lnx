/* one thread t is created(pthread_create) and checked with pthread join on its termination;
 *
 *
 *  */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

static void *tStart(void *arg)
{
	char *s = (char *)arg;
	printf("%s: recv'd arg string %s %ld %d\n", __func__, s, pthread_self(), getpid());

	getchar();
	return (void *)strlen(s);
}

int main()
{
	pthread_t t;
	void *res;
	int ret;

	/*int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
	                          void *(*start_routine) (void *), void *arg);*/

	ret = pthread_create(&t, NULL, tStart, "Hello world\n");
	if (ret != 0) {
		perror("Pthread_create failed \n");
		exit(ret);
	}

	printf("%s: wait for thread completion\n",__func__);

	printf("%s: pid %d\n",__func__,getpid());

	ret = pthread_join(t, &res);
	if (ret != 0)
		printf("%s: pthread join failed\n", __func__);

	
	printf("%s: recv'd on join %ld\n", __func__, (long)res);
//	getchar();
	
	return 0;
}
