/*
 * making joinable thread  to detached thread while thread function is running
 *
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <linux/errno.h>
#include <string.h>

void *tStart(void *null)
{
	pthread_attr_t attr;
	int detachstate;
	
	/* access attrib values in force*/
	pthread_getattr_np(pthread_self(), &attr);
	pthread_attr_getdetachstate(&attr, &detachstate);
	if (detachstate == PTHREAD_CREATE_JOINABLE)
		printf("%s: In Joinable Mode (explicit cleanup)\n", __func__);
	else
		printf("%s: In Detached Mode (auto cleanup)\n", __func__);

	sleep(10);

	pthread_getattr_np(pthread_self(), &attr);
	pthread_attr_getdetachstate(&attr, &detachstate);
	if (detachstate == PTHREAD_CREATE_JOINABLE)
		printf("%s: In Joinable Mode (explicit cleanup)\n", __func__);
	else
		printf("%s: In Detached Mode (auto cleanup)\n", __func__);


	pthread_exit(NULL);
}

int main()
{
	int detach_state, rc;
	pthread_t t;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_create(&t, &attr, tStart, NULL);

	sleep(5);
	/* move thread to detached state */
	rc = pthread_detach(t);
	if(rc != 0)
		printf("%s: detach failed: %s\n",__func__, strerror(rc));

	pthread_exit(NULL);
}
