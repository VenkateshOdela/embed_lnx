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

// pthread Spinlock object
pthread_spinlock_t spin;

// Threads created will execute the following routines

void *thread1(void *p)
{

	printf("TH1: Getting lock ....... \n");
	if (pthread_spin_lock(&spin) != 0)
		printf("\n Error :: Spinlock Failed \n");
	else {
		printf("TH1:  Acquired spinlock, doing work ....... \n");
		sleep(2);
		pthread_spin_unlock(&spin);
		printf("TH1: Releasing spinlock\n");
	}

	pthread_exit(NULL);
}

void *thread2(void *p)
{

	printf("TH2: Getting lock ....... \n");
	if (pthread_spin_lock(&spin) != 0)
		printf("\n Error :: Spinlock Failed \n");
	else {
		printf("TH2:  Acquired spinlock \n  ....... \n");
		printf("TH2:  Releasing lock\n");
		pthread_spin_unlock(&spin);
	}

	pthread_exit(NULL);

}

int main()
{
	pthread_t tcb1, tcb2;
	int rv;

	pthread_spin_init(&spin,
			  PTHREAD_PROCESS_PRIVATE /* PTHREAD_PROCESS_SHARED */
			  );

	rv = pthread_create(&tcb1, NULL, thread1, NULL);
	if (rv)
		puts("Failed to create thread");

	rv = pthread_create(&tcb2, NULL, thread2, NULL);
	if (rv)
		puts("Failed to create thread");

	pthread_join(tcb1, NULL);
	pthread_join(tcb2, NULL);
	puts(" Exit Main");
	return 0;
}
