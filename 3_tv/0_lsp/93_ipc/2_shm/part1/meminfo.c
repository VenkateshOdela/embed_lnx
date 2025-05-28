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
   Demonstrate the use of the SHM_INFO operation to retrieve a 'shminfo'
   structure containing the current usage of System V shared memory resources.

   This program is Linux-specific.
*/
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main()
{
    struct shm_info info;
    int s;

    s = shmctl(0, SHM_INFO, (struct shmid_ds *) &info);
    if (s == -1){
        perror("shmctl");
	exit(1);
      }

    printf("Maximum ID index = %d\n", s);
    printf("shm_tot          = %ld\n", (long) info.shm_tot);
    printf("shm_rss          = %ld\n", (long) info.shm_rss);
    printf("shm_swp          = %ld\n", (long) info.shm_swp);
    printf("swap_attempts    = %ld\n", (long) info.swap_attempts);
    printf("swap_successes   = %ld\n", (long) info.swap_successes);

    exit(EXIT_SUCCESS);
}
