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

/* event_flags.c
   Implement an event flags protocol using System V semaphores.

   See event_flags.h for a summary of the interface.
*/
#include <sys/types.h>
#include <sys/sem.h>
#include "semun.h"              /* Definition of semun union */
#include "event_flags.h"

/* Wait for the specified flag to become "set" (0) */

int waitForEventFlag(int semId, int semNum)
{
    struct sembuf sops;

    sops.sem_num = semNum;
    sops.sem_op = 0;                    /* Wait for semaphore to equal 0 */
    sops.sem_flg = 0;

    /* Waiting for a semaphore to become zero may block, so we
       program to retry if interrupted by a signal handler */

    while (semop(semId, &sops, 1) == -1)
        if (errno != EINTR)
            return -1;
    return 0;
}

/* "Clear" the event flag (give it the value 1) */

int clearEventFlag(int semId, int semNum)
{
    union semun arg;

    arg.val = 1;
    return semctl(semId, semNum, SETVAL, arg);
}

/* "Set" the event flag (give it the value 0) */

int setEventFlag(int semId, int semNum)
{
    union semun arg;

    arg.val = 0;
    return semctl(semId, semNum, SETVAL, arg);
}

/* Get current state of event flag */

int getFlagState(int semId, int semNum, Boolean *isSet)
{
    int sem_val;
    union semun dummy;

    sem_val = semctl(semId, semNum, GETVAL, dummy);
    if (sem_val == -1)
        return -1;

    *isSet = (sem_val == 0) ? TRUE : FALSE;
    return 0;
}
