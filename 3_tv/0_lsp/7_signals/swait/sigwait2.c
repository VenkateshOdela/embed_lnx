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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

void timestamp( char *str ) {
    time_t t;
    time(&t);
    printf( "The time %s is %s\n", str, ctime(&t) );
}

int main( int argc, char *argv[] ) {

    int result = 0;

    sigset_t waitset;
    siginfo_t info;
    struct timespec timeout;

    sigemptyset( &waitset );
    sigaddset( &waitset, SIGALRM );
    sigprocmask( SIG_BLOCK, &waitset, NULL );

    timeout.tv_sec = 4;     /* Number of seconds to wait */
    timeout.tv_nsec =30;  /* Number of nanoseconds to wait */


    alarm(5);

    timestamp( "before sigtimedwait()" );

    result = sigtimedwait( &waitset, &info, &timeout );
    if( result <  0 ) {
	printf("sigtimedwait failed : \n");
        exit(1);
    }

    if(info.si_signo == SIGALARM){
    	printf( "sigtimedwait returned for signal %d\n",info.si_signo );
    	timestamp( "after sigtimedwait()" );
    }

    return( result );
}
