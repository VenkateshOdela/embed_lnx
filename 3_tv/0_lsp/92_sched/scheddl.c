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


#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <time.h>
#include <linux/types.h>
#include <sched.h>
#include <stdlib.h>

#define SCHED_DEADLINE  6

/* XXX use the proper syscall numbers */

/* __NR_sched_setattr number */
#ifndef __NR_sched_setattr
#ifdef __x86_64__
#define __NR_sched_setattr              314
#endif

#ifdef __i386__
#define __NR_sched_setattr              351
#endif

#ifdef __arm__
#define __NR_sched_setattr              380
#endif
#endif

struct sched_attr {
        __u32 size;

        __u32 sched_policy;
        __u64 sched_flags;

        /* SCHED_NORMAL, SCHED_BATCH */
        __s32 sched_nice;

        /* SCHED_FIFO, SCHED_RR */
        __u32 sched_priority;

        /* SCHED_DEADLINE */
        __u64 sched_runtime;
        __u64 sched_deadline;
        __u64 sched_period;
};

int sched_setattr(pid_t pid,
                      const struct sched_attr *attr,
                      unsigned int flags)
{
        return syscall(__NR_sched_setattr, pid, attr, flags);
}

int main()
{
  struct sched_attr attr;
  int ret;
  int inc = 0;
  int flags = 0;

   memset(&attr, 0, sizeof(attr));
   attr.size = sizeof(attr);

   /* This creates a 200ms / 1s reservation */
   attr.sched_policy   = SCHED_DEADLINE;
   attr.sched_runtime  =  200000000;
   attr.sched_deadline = attr.sched_period = 1000000000;

   ret = sched_setattr(0, &attr, flags);
        if (ret < 0) {
            perror("sched_setattr failed to set the priorities");
            exit(-1);
        }

 /* code to carry out desired computation without blocking */
	for(;;) {
            inc += 5;
	    printf("%s: Inc: %d\n", __func__, inc);
            /*
	     * Notify the scheduler the end of the computation
             * This syscall will block until the next replenishment
             */
	    sched_yield();
        }
  return 0;
}
