/*
 * joinable thread t is created.
 * detached thread t1 is created with attribute PTHREAD_CREATE_DETACHED
 *
 * attributes are passed in second argument of pthread_create;
 * pthread_attr_t attr;
 * pthread_attr_init(&attr);
 * pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
 * pthread_create(&t1, &attr, detachedThread, NULL);
 *
 * pthred_join is succesful for joinable thread
 * pthread_join is failed for detached thread
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

int *ret;
static void *joinableThread(void *x)
{
	int i = 0;
	ret = (int *)x;

	unsigned int tid;
	tid = (unsigned int)pthread_self();
	printf("%s:Joinable Thread id %u\n", __func__, tid);
	for (i = 0; i < 10; i++) {
		printf("%s: In iteration %d\n", __func__, i);
		sleep(2);
	}
	return (void *)ret;
}

static void *detachedThread(void *x)
{
	int i = 0;
	unsigned int tid;
	tid = (unsigned int)pthread_self();

	printf("%s:detached Thread id %u\n", __func__, tid);

	for (i = 0; i < 10; i++) {
		printf("%s: In iteration %d\n", __func__, i);
		sleep(5);
	}

	//in case of child, pthread_exit is exit call;
	pthread_exit(NULL);
}

int main()
{
	pthread_t t, t1;
	pthread_attr_t attr;
	int ret;
	void *res;

	/* init defaults */
	ret = pthread_attr_init(&attr);
	if (ret != 0)
		perror("pthread_attr_init");

	ret = pthread_create(&t, NULL, joinableThread, (void *)10);
	if (ret != 0) {
		perror("pthread_create");
		exit(ret);
	}

	/* assign detached state */
	ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (ret != 0)
		perror("pthread_attr_getdetachstate");

	ret = pthread_create(&t1, &attr, detachedThread, NULL);
	if (ret != 0) {
		perror("pthread_create");
		exit(ret);
	}

	ret = pthread_join(t, &res);
	if (ret != 0)
		printf("%s: pthread_join failed on Joinable thread(t): %s\n",
		       __func__, strerror(ret));

	printf("%s: Joinable Thread 't' returned %ld\n", __func__, (long)res);

	printf("%s: Attempt to join with 't1'(detached thread)\n", __func__);
	ret = pthread_join(t1, &res);
	if (ret != 0)
		printf("%s: pthread_join failed on detached thread(t1): %s\n",
		       __func__, strerror(ret));

	// in case of parent, pthread_exit is blocking call
	// pthread exit suspends the parent thread until all another threads termination.
	printf("suspended at parent pthread_exit until all another threads termination\n");
	pthread_exit(NULL);
}
