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
#include <unistd.h>


pthread_mutex_t mutex;

 
void * writer(void *p)
{
	if(pthread_mutex_lock(&mutex)==0)
	{
		printf(" Writer: lock acquired\n");
		if(pthread_mutex_lock(&mutex)==0)
			printf("\n Recursive lock attempt succeded \n");
		printf(" updating commits .......\n");
		sleep(2);
		printf(" Done : exit from mutex block\n");
		pthread_mutex_unlock(&mutex);
//		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);	
}

void * reader(void *p)
{
	if(pthread_mutex_lock(&mutex)==0)
	{
          printf(" Reader : lock acquired\n");
	  printf(" Reading updates ...... \n");
	  sleep(2);
	  printf(" Done: exit from mutex block \n");
	  pthread_mutex_unlock(&mutex);
        }
        pthread_exit(NULL);
}

int main()
{
	pthread_t tcb1,tcb2;

	// Mutex Attribute object
	pthread_mutexattr_t attr;
	int rv;

	// initialise mutex attribute object 
	pthread_mutexattr_init(&attr);

	// Set mutex type to PTHREAD_MUTEX_RECURSIVE
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mutex,&attr);
	
	rv = pthread_create(&tcb1, NULL, writer, NULL);
	if(rv)
	    puts("Failed to create thread");

	rv = pthread_create(&tcb2, NULL, reader, NULL);
        if(rv)
             puts("Failed to create thread");

	pthread_exit(NULL);
}
