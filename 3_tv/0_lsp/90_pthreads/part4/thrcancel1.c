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

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void *thread_func1(void *ignored_argument)
{
	int s;
	sleep(3);
	pthread_mutex_lock(&mutex);
	printf("Thread 1 acquire mutex \n");
//	while(1)
//		sleep(10);		/* Should get canceled while we sleep */
	pthread_mutex_unlock(&mutex);
	printf("Thread 1 releases mutex \n");

	/* Should never get here */

	return NULL;
}


static void *thread_func2(void *ignored_argument)
{
        int s;
	printf("\n %s: \n",__func__);
        sleep(5);
        /* sleep() is a cancellation point */
	printf("Thread 2 , waiting for mutex\n");
	pthread_mutex_lock(&mutex);
        printf("Thread 2 acquire mutex \n");
        pthread_mutex_unlock(&mutex);
        printf("Thread 2 releases mutex \n");


        return NULL;
}

int main(void)
{
	pthread_t thr1,thr2;
	void *res;
	int s;

	/* Start a thread and then send it a cancellation request */

	s = pthread_create(&thr1, NULL, &thread_func1, NULL);
	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	s = pthread_create(&thr2, NULL, &thread_func2, NULL);
        if (s != 0) {
                perror("pthread_create");
                exit(EXIT_FAILURE);
        }
	sleep(2);		/* Give thread a chance to get started */

	printf("main(): sending cancellation request\n");

	s = pthread_cancel(thr1);
	if (s != 0) {
		perror("pthread_cancel");
                exit(EXIT_FAILURE);
	}

	/* Join with thread to see what its exit status was */

	s = pthread_join(thr1, &res);
	if (s != 0) {
		perror("pthread_join");
                exit(EXIT_FAILURE);
	}
	if (res == PTHREAD_CANCELED)
		printf("main(): thread was canceled\n");
	else
		printf("main(): thread wasn't canceled (shouldn't happen!)\n");
	
	s = pthread_join(thr2, &res);
        if (s != 0) {
                perror("pthread_join");
                exit(EXIT_FAILURE);
        }

	
	exit(EXIT_SUCCESS);
}
