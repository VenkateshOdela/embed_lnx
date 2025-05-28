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

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

pthread_barrier_t barrier;
static void *t1(void *arg)
{

	printf("T1:initiating job1\n");
	printf("T1: ......\n");
	printf("T1: Done... hitting barrier \n");
	pthread_barrier_wait(&barrier);
	printf(" T1: Resume from barrier ....\n");
}

static void *t2(void *arg)
{
	printf("T2: initiating job2\n");
	sleep(3);
	printf("T2: .....\n");
	printf("T2: Done... hitting barrier\n");
	pthread_barrier_wait(&barrier);
	printf("T2: Resume from barrier ...\n");
}

static void *t3(void *arg)
{

	printf("T3: initiating job3\n");
	sleep(4);
	printf("T3: .....\n");
	pthread_barrier_wait(&barrier);
	printf("T3: Done... hitting barrier\n");
	printf("T3: Resume from barrier ....\n");
}

int main()
{
	pthread_t tcb1, tcb2, tcb3;

	pthread_barrier_init(&barrier, NULL, 3);

	pthread_create(&tcb1, NULL, t1, NULL);
	pthread_create(&tcb2, NULL, t2, NULL);
	pthread_create(&tcb3, NULL, t3, NULL);
	pthread_exit(NULL);
}
