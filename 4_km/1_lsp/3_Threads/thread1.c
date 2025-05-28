#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void *thread_function(void *arg);

char message[] = "venkatesh"; // this is passed as thread function argument

int main() {
    int res;
    char buff[20];

    pthread_t a_thread; // thread name
    void *thread_result; // this is argument in pthread_join() function

    /*
    The  pthread_create()  function  starts a new thread in the calling process.
    The new thread starts execution by invoking  start_routine();
    arg  is  passed  as  the  sole  argument  of start_routine().*/
    /*int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                              void *(*start_routine) (void *), void *arg); */
    pthread_t thread_id;
    printf("thread id =%lu \n",pthread_self());

    res = pthread_create(&a_thread, NULL, thread_function, (void *)message);
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    /*The  pthread_join() function waits for the thread specified by
     thread to terminate.*/
    printf("Waiting for thread to finish...\n");
    res = pthread_join(a_thread, &thread_result);
    if (res != 0) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf("Thread joined, it returned %s\n", (char *)thread_result);
    printf("Message is now %s\n", message);
    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg) {

	int count = 0;
	pthread_t thread_id;
    printf("thread_function is running. Argument was %s\n", (char *)arg);
    printf("thread id =%lu \n",pthread_self());
    while(count<10)
    {
    	count++;
    	sleep(1);
    	printf("%dseconds\n",count);
    }

    strcpy(message, "odela");
    /*
    The  pthread_exit()  function  terminates  the calling thread and returns a value
    via retval that (if the thread is joinable) is available to another thread in  the
	same  process  that calls pthread_join(3). */

    //pthread_exit - terminate calling thread
    pthread_exit("Thank you for the CPU time");
}

/*
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);

int pthread_join(pthread_t thread, void **retval);

void pthread_exit(void *retval);
*/

/* pthread creation, pthread joining, pthread exit */
