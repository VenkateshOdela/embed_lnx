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

/* 

   Decrease the value of a POSIX named semaphore.

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>

#define NAME "/mysem"

#define DEATH(mess) { perror(mess); exit(errno); }

int main()
{
    sem_t *sem;

    sem = sem_open(NAME , 0);
    if (sem == SEM_FAILED)
        DEATH ("sem_open");

    if (sem_wait(sem) == -1)
        DEATH ("sem_wait");
   
    if (sem_wait(sem) == -1)
        DEATH ("sem_wait");

    printf("%ld sem_wait() succeeded\n", (long) getpid());
    exit(EXIT_SUCCESS);
}
