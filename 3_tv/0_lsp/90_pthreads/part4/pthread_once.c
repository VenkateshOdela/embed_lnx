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
#include <malloc.h>

pthread_once_t once = PTHREAD_ONCE_INIT;
pthread_mutex_t *mymutex;

void *myinit() // run  once
{
	printf("\n I am in init fun called by %u\n", pthread_self());
	mymutex = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mymutex, NULL);
}

void *  mythread(void *arg )
{
	int i = 0;
	printf("\n %s: \n",__func__);
	pthread_once(&once,(void *)myinit);
	pthread_mutex_lock(mymutex);
	while(i<10)
		printf("Thread  %u in iteration %d \n", pthread_self(),i++);
	pthread_mutex_unlock(mymutex);
	
}

int main()
{
	int id = 0;
	pthread_t thread[3];

	pthread_create(&thread[0],NULL,mythread,NULL);
	pthread_create(&thread[1],NULL,mythread,NULL);	
	pthread_create(&thread[2],NULL,mythread,NULL);	

/*	while(id < 3)
	{
		pthread_join(thread[id],NULL);
		id++;
	}*/
	pthread_exit(NULL);
	return EXIT_SUCCESS;
}
	
