/*
 * -- a thread t is created with attributes with scheduling parameters
 *  PTHREAD_EXPLICIT_SCHED ,policy=SCHED_FIFO and priority=10;
 *  --then for same thread t scheduling parameters changed to policy=SCHED_RR and priority=20;
 *
 * Scheduling : (man 7 sched )
 *
 * we can schedule only threads, not processes;
 *
 * attributes can be assigned when a thread is being created;
 *
 * pthread_t t; // thread name for identification
 * pthread_attr_t attr;
 *
 * pthread_attr_init(&attr);
 *
 * pthread_attr_init(&attr) function initializes the thread attributes object
 * pointed by attr with default attribute values
 *
 *
 * struct sched_param param;
 *
 * - inheritance (PTHREAD_INHERIT_SCHED , PTHREAD_EXPLICIT_SCHED)
 * - policy ( 0 - SCHED_OTHERS     1 - SCHED_FIFO          2 - SCHED_RR )
 * - priority
 *
 * switch off sched inheritence from parent
 * pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
 *
	Assign Sched policy and priority
	policy = SCHED_FIFO;
	pthread_attr_setschedpolicy(&attr, policy);

	param.sched_priority = 10;
	pthread_attr_setschedparam(&attr, &param);
 *
 *
 *
 * pthread_attr_destroy(&attr);
 * if a thread attributes are no longer required call pthread_attr_destroy;
 *
 *  use sudo ./a.out while executing;
 *
 *
 *
 */

#define _GNU_SOURCE 

#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

void *tStart(void *null)
{
	int policy;
	struct sched_param param;
	cpu_set_t cset;

	pthread_getschedparam(pthread_self(), &policy, &param);
	// 0 - SCHED_OTHERS     1 - SCHED_FIFO          2 - SCHED_RR
	printf("%s: Current Policy : %d Priority : %d\n", __func__, policy,
	       param.sched_priority);

	sleep(5);

	
	CPU_ZERO( &cset );
    CPU_SET( 0, &cset);
    CPU_SET( 1, &cset);

	pthread_setaffinity_np(pthread_self(),sizeof(cset), &cset);

	pthread_getschedparam(pthread_self(), &policy, &param);
	// 0 - SCHED_OTHERS     1 - SCHED_FIFO          2 - SCHED_RR
	printf("%s: Current Policy : %d Priority : %d\n", __func__, policy,
	       param.sched_priority);

	getchar();
	pthread_exit(NULL);
}

int main()
{
	int policy, rc;
	pthread_t t;
	pthread_attr_t attr;
	struct sched_param param;

	pthread_attr_init(&attr);

	/* switch off sched inheritence from parent */
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

	/* Assign Sched policy and priority */
	policy = SCHED_FIFO;
	pthread_attr_setschedpolicy(&attr, policy);

	param.sched_priority = 10;
	pthread_attr_setschedparam(&attr, &param);

	printf("calling thread function \n");
	/* create thread with choosen attribs */
	pthread_create(&t, &attr, tStart, NULL);

	/* destroy attribute object */
	pthread_attr_destroy(&attr);

	sleep(2);


	policy = SCHED_RR;
	param.sched_priority = 20;
	pthread_setschedparam(t, policy, &param); // changing policy

	pthread_exit(NULL);
}

