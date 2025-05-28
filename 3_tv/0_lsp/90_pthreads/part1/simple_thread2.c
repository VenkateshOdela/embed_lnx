/* thread t2 is created from thread t1
 * first thread t2 should exit, then only thread t1 exit.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

static void *t2_Start(void *arg)
{
	int i = 0;
	for (i = 0; i < 10; i++) {
		printf("%s: In iteration %d\n", __func__, i);
		sleep(2);
	}
	getchar();
	pthread_exit(NULL);

}

static void *t1_Start(void *arg)
{
	pthread_t t2;
	int ret;

	printf("%s: initiating thread t2\n", __func__);

	ret = pthread_create(&t2, NULL, t2_Start, NULL);
	if (ret != 0) {
		perror("Pthread_create \n");
		exit(ret);
	}
	ret = pthread_join(t2, NULL);
	if (ret != 0)
		printf("%s: Join failed :%s\n", __func__, strerror(ret));

	printf("%s:Thread t2 returned\n", __func__);
	pthread_exit(NULL);
}

int main()
{
	pthread_t t1, t2;
	void *res;
	int ret;

	ret = pthread_create(&t1, NULL, t1_Start, NULL);
	if (ret != 0) {
		perror("Pthread_create \n");
		exit(ret);
	}
	printf("%s:Thread 't1' initialised, wait for completion\n", __func__);

	ret = pthread_join(t1, NULL);
	if (ret != 0)
		printf("%s: Join failed :%s\n", __func__, strerror(ret));

	printf("%s:Thread t1 returned\n", __func__);
	return 0;
}
