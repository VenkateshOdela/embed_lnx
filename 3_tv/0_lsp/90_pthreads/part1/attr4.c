/* thread t is created with scheduling policy=SCHED_FIFO and Priority=5
 * thread t1 is created with scheduling policy=SCHED_RR and Priority=10
 *
 * Note :
 * When  a thread attributes object is no longer required, it should be destroyed using
 * the pthread_attr_destroy() function.  Destroying a thread attributes object has no effect
 * on threads that were created using that object.
 */

#include <stdio.h>
#include <linux/errno.h>
#include <pthread.h>

void *t1_start(void *null)
{
	int policy, i;
	struct sched_param param;
	for (i = 0; i < 100; i++) {
		pthread_getschedparam(pthread_self(), &policy, &param);
		// 0 - SCHED_OTHERS     1 - SCHED_FIFO          2 - SCHED_RR
		printf("%s : %u:Thread loop count :%d Policy : %d Priority : %d\n",
		       __func__,(unsigned int)pthread_self(), i, policy,
		       param.sched_priority);
		sleep(1);
	}

}
void *t_start(void *null)
{
	int policy, i;
	struct sched_param param;
	for (i = 0; i < 100; i++) {
		pthread_getschedparam(pthread_self(), &policy, &param);
		// 0 - SCHED_OTHERS     1 - SCHED_FIFO          2 - SCHED_RR
		printf("%s :%u:Thread loop count :%d Policy : %d Priority : %d\n",
		       __func__,(unsigned int)pthread_self(), i, policy,
		       param.sched_priority);
		sleep(1);
	}

}

int main()
{
	int  policy,  rc;
	pthread_t t, t1;
	pthread_attr_t attr;
	struct sched_param param;

	/* The  pthread_attr_init()  function  initializes the thread attributes object
	 * pointed to by attr with default attribute values.
	 */
	pthread_attr_init(&attr);

	/* switch off sched inheritence from parent */
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

	/* Now assign Sched policy and priority */
	policy = SCHED_FIFO;
	pthread_attr_setschedpolicy(&attr, policy);

	param.sched_priority = 5;
	pthread_attr_setschedparam(&attr, &param);

	/* create thread with choosen attribs */
	pthread_create(&t, &attr, t_start, NULL);


	/* When  a thread attributes object is no longer required, it should be destroyed using
	 * the pthread_attr_destroy() function.  Destroying
	 * a thread attributes object has no effect on threads that were created using that object.
	 */

	/* destroy attribute object */
	pthread_attr_destroy(&attr);

	policy = SCHED_RR;
	pthread_attr_setschedpolicy(&attr, policy);

	param.sched_priority = 10;
	pthread_attr_setschedparam(&attr, &param);

	pthread_create(&t1, &attr, t1_start, NULL);

	/* destroy attribute object */
	pthread_attr_destroy(&attr);

	pthread_exit(NULL);

}
