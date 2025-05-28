/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2020.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 31-2 */

/* strerror_test.c

   A program to test whether the implementation of strerror() thread-safe.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>                 /* Get declaration of strerror() */
#include <pthread.h>
#include <errno.h>

static void *
threadFunc(void *arg)
{
    char *str;

    printf("Other thread about to call strerror()\n");
    str = strerror(EPERM);
    printf("Other thread: address of str(%p) str (%p) = %s\n",&str, str, str);
    str = strerror(EINVAL);
    printf("Other thread: address of str(%p) str (%p) = %s\n",&str, str, str);
    str = strerror(ENOMEM);
    printf("Other thread: address of str(%p) str (%p) = %s\n",&str, str, str);

    return NULL;
}

int
main(int argc, char *argv[])
{
    pthread_t t;
    int s;
    char *str;

    str = strerror(EINVAL);
    printf("Main thread has called strerror()\n");

    s = pthread_create(&t, NULL, threadFunc, NULL);
    if (s != 0){
        perror("pthread_create :");
    	goto out;
    }

    s = pthread_join(t, NULL);
    if (s != 0){
        perror("pthread_join: ");
    	goto out;
    }
    /* If strerror() is not thread-safe, then the output of this printf() be
       the same as that produced by the analogous printf() in threadFunc() */

    printf("Main thread: address of str (%p)  str (%p) = %s\n", &str, str, str);

    return 0;
out:
    exit(1);
}
