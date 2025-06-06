/*
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * ******************************************************************************
 * This program is part of the source code provided with "Linux Systems Programming" Course
 * (c) 2019- 2021 Techveda, Hyderabad, India
 * website : www.techveda.org
 * Author/Trainer:  Raghu Bharadwaj
 * ******************************************************************************
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 */

#define _GNU_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>

// pthread mutex object
pthread_mutex_t mutex;

void recover(void)
{
	printf("\n Performing Recovery by Thread : %ld \n",pthread_self());
	/* erase owner record  */
	pthread_mutex_consistent_np(&mutex);
	/* reset shared data to valid state before unlocking mutex */
	pthread_mutex_unlock(&mutex);	
	printf("\n Recovery completed by Thread : %ld \n",pthread_self());
	pthread_mutex_lock(&mutex);
}
void * writer(void *p)
{
	if(pthread_mutex_lock(&mutex)==0)
	{
		printf("\nWrite thread: updating commits....\n");
		pthread_exit(NULL);
	}
}

void * reader(void *p)
{
	int rv;
	sleep(1);
	rv = pthread_mutex_lock(&mutex);
	 if(rv==EOWNERDEAD)
        {
                printf("\n Owner Dead identified by : %ld\n",pthread_self());
		/* Reset shared data and mutex to valid state */
		recover();
        }
        printf(" Lock acquired by : %ld\n",pthread_self());
	printf(" Reading ...... \n Done \n Thread %ld : exiting crictical section\n",pthread_self());
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}
int main ()
{
	pthread_t tcb1,tcb2,tcb3;

	/* Mutex Attribute object */
	pthread_mutexattr_t attr;
	int rv;

	/* initializing mutex attribute object  */
	pthread_mutexattr_init(&attr);

	/* enable owner death validation  */
	pthread_mutexattr_setrobust_np(&attr, PTHREAD_MUTEX_ROBUST_NP);
	pthread_mutex_init(&mutex,&attr);

	
	// Routine shell create a new thread
	rv = pthread_create(&tcb1, NULL, writer, NULL);
	if(rv)
		puts("Failed to create thread");

	rv = pthread_create(&tcb2, NULL, reader, NULL);
        if(rv)
                puts("Failed to create thread");
	
	rv = pthread_create(&tcb3, NULL, reader, NULL);
        if(rv)
                puts("Failed to create thread");

	// suspend execution of the intial thread until the target threads terminate
	pthread_exit(NULL);
}
