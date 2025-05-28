/*
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * ******************************************************************************
 * This program is part of the source code provided with
 * "Linux Systems Programming Essentials" Course offered by TECHVEDA(c) 2020 - 2022
 * Course page : https://www.techveda.org/linuxsysprg
 * Trainer:  Raghu Bharadwaj
 * ******************************************************************************
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>

static void threadFunc(union sigval sv)
{
   printf("Thread create with id : %ld\n", pthread_self());
   pthread_exit(NULL);
}

int main()

{
	struct sigevent sev;
	timer_t tid;
	struct itimerspec ts;
	int s, j;

	sev.sigev_notify = SIGEV_THREAD;	/* Notify via thread */
	sev.sigev_notify_function = threadFunc;	/* Thread start function */
	sev.sigev_notify_attributes = NULL;	/* Could be pointer to pthread_attr_t structure */

	/* Create and start timer */

	ts.it_interval.tv_sec = 2;
	ts.it_interval.tv_nsec = 10;
	ts.it_value.tv_sec = 1;
	ts.it_value.tv_nsec = 20;

	if (timer_create(CLOCK_REALTIME, &sev, &tid) == -1) {
		perror("timer_create");
		exit(1);
	}

	if (timer_settime(tid, 0, &ts, NULL) == -1)
		perror("timer_settime");


	for (;;) {
	sleep(1);
	printf(" This is a pointless message\n");
	}
}
