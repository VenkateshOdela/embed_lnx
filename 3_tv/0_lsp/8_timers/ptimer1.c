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

#define BUF_SIZE 1000

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

char * currTime(const char *);

static void handler(int sig, siginfo_t *si, void *uc)
{
    sleep(3);
    printf("[%s] Got signal %d\n", currTime("%T"), sig);
    printf("    sival_int          = %d\n", si->si_value.sival_int);
    printf("    timer_getoverrun() = %d\n",
            timer_getoverrun((timer_t) si->si_value.sival_ptr));
    
}

int main()
{
    struct itimerspec ts;
    timer_t tid;
    int j;
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO ;  // enable siginfo_t arg
    sa.sa_sigaction = handler;  // assign handler to sigaction function pointer
    if (sigaction(SIGALRM, &sa, NULL) == -1)
        perror("sigaction");

    if (timer_create(CLOCK_MONOTONIC, NULL, &tid) == -1)
        perror("timer_create");
    printf("timer ID = %ld\n", (long) tid);

    ts.it_value.tv_sec = 1;
    ts.it_value.tv_nsec = 0;
    ts.it_interval.tv_sec = 1; // periodic interval ( 0 single shot)
    ts.it_interval.tv_nsec = 0;

    if (timer_settime(tid, 0, &ts, NULL) == -1)
        perror("timer_settime");

/*    for (j = 0; ; j++)
        pause(); */
	while(1);
}




/* Return a string containing the current time formatted according to
 * the specification in 'format' (see strftime(3) for specifiers).
 * If 'format' is NULL, we use "%c" as a specifier (which gives the'
 * date and time as for ctime(3), but without the trailing newline).
 * Returns NULL on error. */

char * currTime(const char *format)
{
    static char buf[BUF_SIZE];  /* Nonreentrant */
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

