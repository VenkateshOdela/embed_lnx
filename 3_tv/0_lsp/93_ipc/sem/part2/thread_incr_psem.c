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

/* thread_incr_psem.c

   Use a POSIX unnamed semaphore to synchronize access by two threads to
   a global variable.

*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define DEATH(mess) { perror(mess); exit(errno); }

static int glob = 0;
static sem_t sem;

static void *threadFunc(void *arg)
{
    int loops = *((int *) arg);
    int loc, j;
	printf("%s: \n",__func__);
    for (j = 0; j < loops; j++) {
      if (sem_wait(&sem) == -1)
            DEATH ("sem_wait");

        loc = glob;
        loc++;
        glob = loc;

      if (sem_post(&sem) == -1)
            DEATH ("sem_post");
    }

    return NULL;
}

int main()
{
    pthread_t t1, t2;
    int loops, s;

    loops = 10000000;

    /* Initialize a thread-shared mutex with the value 1 */

    if (sem_init(&sem, 0, 1) == -1)
        DEATH("sem_init");

    /* Create two threads that increment 'glob' */

    s = pthread_create(&t1, NULL, threadFunc, &loops);
    if (s != 0)
        DEATH( "pthread_create");
    s = pthread_create(&t2, NULL, threadFunc, &loops);
    if (s != 0)
        DEATH ("pthread_create");

    /* Wait for threads to terminate */

    s = pthread_join(t1, NULL);
    if (s != 0)
        DEATH ("pthread_join");
    s = pthread_join(t2, NULL);
    if (s != 0)
        DEATH( "pthread_join");

    printf("glob = %d\n", glob);
    exit(EXIT_SUCCESS);
}
