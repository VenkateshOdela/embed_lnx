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

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; // lock protecting data

static int avail = 0;// shared data unit 

/* producer code */
static void *threadFunc(void *arg)
{
	int cnt = *((int *)arg);
	int j;

	for (j = 0; j < cnt; j++) {
		sleep(1);
		pthread_mutex_lock(&mtx);
		avail++;	
		pthread_mutex_unlock(&mtx);
	}

	return NULL;
}

int main()
{
	pthread_t tid;
	int ret, j;
	int totRequired;	
				   
	int numConsumed = 0;	
	Boolean done = FALSE;

	totRequired = 20;

	ret = pthread_create(&tid, NULL, threadFunc, &totRequired);
	if (ret != 0)
		perror("pthread_create:      ");

	for (;;) {

		/* acquire lock on shared data */
		pthread_mutex_lock(&mtx);

		/* check if data is produced , consume if ready */
		while (avail > 0) { 
		    numConsumed++;
		    avail--;
		    printf("%s:Data Unit Consumed=%d\n",__func__, numConsumed);
		    done = numConsumed >= totRequired;
		}
		/* unlock mutex */
		pthread_mutex_unlock(&mtx);

		if (done)
			break;

	}

	pthread_exit(NULL);
}
