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
#include <stdlib.h>

typedef enum { FALSE, TRUE } Boolean;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int avail = 0;

static void *threadFunc(void *arg)
{
	int cnt = *((int *)arg);
	int j, ret;

	for (j = 0; j < cnt; j++) {
		/* Code to produce units */
		sleep(3);
		pthread_mutex_lock(&mtx);

		avail++; /* Let consumer know another unit is available */

		pthread_mutex_unlock(&mtx);
		ret = pthread_cond_broadcast(&cond);/* Wake sleeping consumer */
		if (ret != 0)
			perror("pthread_cond_signal");

	}

	return NULL;
}

int main()
{
	pthread_t tid;
	int ret, j;
	int totRequired;	/* Total number of units that all
				   threads will produce */
	int numConsumed = 0;	/* Total units so far consumer */
	Boolean done = FALSE;

	totRequired = 10;

	/* launch producer thread */

	ret = pthread_create(&tid, NULL, threadFunc, &totRequired);
	if (ret != 0)
		perror("pthread_create:      ");

	/* Consumer code to process produced units */

	/* Use a polling loop to check for available units */
	for (;;) {
		pthread_mutex_lock(&mtx);
		while (avail == 0) {	/* wait for units to consume */
			ret = pthread_cond_wait(&cond, &mtx);
			/* 1. unlock mutex
			   2. waits for condition to be signalled
			   3. locks mutex
			*/
			if (ret != 0)
				perror("pthread_cond_wait");
		}

		/* At this point, 'mtx' is locked... */

		while (avail > 0) {	/* Consume all available units */
			/* Do something with produced unit */
			numConsumed++;
			avail--;
			printf("numConsumed=%d\n", numConsumed);
			done = numConsumed >= totRequired;
		}

		pthread_mutex_unlock(&mtx);

		if (done)
			break;

	}

	pthread_exit(NULL);
	exit(EXIT_SUCCESS);
}
