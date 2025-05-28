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

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
// Can also use pthread_rwlock_init (pthread_rwlock_t *, pthread_rwlockattr_t *);

void *writer(void *p)
{
	printf("\n %s: \n", __func__);
	/* writer lock interfaces available
	   pthread_rwlock_wrlock (pthread_rwlock_t *);
	   pthread_rwlock_trywrlock (pthread_rwlock_t *);
	   pthread_rwlock_timedwrlock (pthread_rwlock_t *,struct timespec *); 
        */
           printf("WRITER: waiting on lock \n");
           sleep(1);
	if (pthread_rwlock_wrlock(&rwlock) != 0)
		printf("\n Error :: Failed to Acquire Write Lock\n");
	else {
		printf("Successfully Acquired Write Lock\nWriting ...\n");
		sleep(1);
		printf("Writer: releasing write lock \n");
		pthread_rwlock_unlock(&rwlock);
	}
	pthread_exit(NULL);
}

void *reader(void *p)
{
	printf("\n %s: \n", __func__);
	/*
	   Reader lock interfaces available
	   pthread_rwlock_rdlock (pthread_rwlock_t *);
	   pthread_rwlock_tryrdlock (pthread_rwlock_t *);
	   pthread_rwlock_timedrdlock (pthread_rwlock_t *,struct timespec *); 
        */

	if (pthread_rwlock_rdlock(&rwlock) != 0)
		printf("Error: Failed to Acquire Read Lock by %ld \n",
		       pthread_self());
	else {

		printf("Reader Thread %ld : Read Lock acquired\n",
		       pthread_self());
		sleep(3);
		printf("Reader Thread %ld :Releasing read lock \n",
		       pthread_self());
		pthread_rwlock_unlock(&rwlock);
	}
	pthread_exit(NULL);
}

int main()
{
	pthread_t tcb1, tcb2, tcb3, tcb4;
	int rv;

	rv = pthread_create(&tcb1, NULL, writer, NULL);
	if (rv)
		puts("Failed to create thread");

	rv = pthread_create(&tcb4, NULL, writer, NULL);
	if (rv)
		puts("Failed to create thread");

	rv = pthread_create(&tcb2, NULL, reader, NULL);
	if (rv)
		puts("Failed to create thread");

	rv = pthread_create(&tcb3, NULL, reader, NULL);
	if (rv)
		puts("Failed to create thread");

	pthread_join(tcb1, NULL);
	pthread_join(tcb2, NULL);
	pthread_join(tcb3, NULL);
	pthread_join(tcb4, NULL);
	return 0;
}
