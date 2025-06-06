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

#define _POSIX_C_SOURCE 199309
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#define TIMER_SIG 64           /* Our timer notification signal */
#define BUF_SIZE 1000

char * currTime(const char *);

static void handler(int sig, siginfo_t *si, void *uc)
{
 	timer_t *tidptr;
	printf(" signal recvd %d\n", sig);
    	tidptr = si->si_value.sival_ptr;
    	printf("    timer_getoverrun() = %d\n", timer_getoverrun(*tidptr));
}

int main()
{
    struct itimerspec ts;
    struct sigaction  sa;
    struct sigevent   sev;
    timer_t tid;
    int j;

    /* Establish handler for notification signal */

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGRTMAX, &sa, NULL) == -1){
        perror("sigaction");
	}

    /* Create and start timer  */

    ts.it_interval.tv_sec = 2;
    ts.it_interval.tv_nsec = 10;
    ts.it_value.tv_sec = 1;
    ts.it_value.tv_nsec = 20;

   /* pass timer id to handler */
    sev.sigev_value.sival_ptr = &tid;
    sev.sigev_notify = SIGEV_SIGNAL;    /* Notify via signal */
    sev.sigev_signo = SIGRTMAX;        /* Notify using this signal */

    if (timer_create(CLOCK_REALTIME, &sev, &tid) == -1){
            perror("timer_create");
	    exit(1);
	   }

        if (timer_settime(tid, 0, &ts, NULL) == -1)
            perror("timer_settime");

    for (;;)                            /* Wait for incoming timer signals */
        pause();
}

char * currTime(const char *format)
{
    static char buf[BUF_SIZE];  
    time_t t;
    size_t s;
    struct tm *tm;

    t = time(NULL);
    tm = localtime(&t);
    if (tm == NULL)
        return NULL;

    s = strftime(buf, BUF_SIZE, (format != NULL) ? format : "%c", tm);

    return (s == 0) ? NULL : buf;
}

