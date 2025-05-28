#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/ioctl.h>

void *thread_function(void *arg);
sem_t bin_sem; // the bin_sem is initialized Globally.

#define WORK_SIZE 1024
char work_area[WORK_SIZE];

int main() {
    int res,nread;
    pthread_t a_thread; // thread name
    void *thread_result; //argumnet in pthread_join()
    
    /* int sem_init(sem_t *sem, int pshared, unsigned int value); */

    /* sem_init()  initializes  the  unnamed semaphore at the address
       pointed to by 'sem'.  The 'value' argument specifies the initial value
       for the semaphore. */

    /* The 'pshared' argument indicates whether this semaphore is to be shared
     * between the threads of a process, or between processes.

       If  'pshared'  has the value 0, then the semaphore is shared between the
       threads of a process,and should be located at some address that is visible
       to all threads (e.g., a  global  variable, or a variable allocated dynamically
       on the heap). */

    res = sem_init(&bin_sem, 0, 0);
    if (res != 0) {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
    res = pthread_create(&a_thread, NULL, thread_function, NULL);
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Input some text. Enter 'end' to finish\n");
    while(strncmp("end", work_area, 3) != 0) {
        fgets(work_area, WORK_SIZE, stdin);
//	ioctl(0,FIONREAD,&nread);
//	read(0,work_area,nread);
     /*
      * sem_post - unlock a semaphore
      * int sem_post(sem_t *sem);
      *
      *  sem_post()  increments  (unlocks) the semaphore pointed to by sem.
      *  If the semaphore's value consequently becomes greater than  zero,  then  another  process  or  thread  blocked  in  a
         sem_wait(3) call will be woken up and proceed to lock the semaphore.
      */
        sem_post(&bin_sem);
    }
    printf("\nWaiting for thread to finish...\n");
    res = pthread_join(a_thread, &thread_result);
    if (res != 0) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf("Thread joined\n");
    sem_destroy(&bin_sem);
    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg) {
	/* sem_wait, sem_timedwait, sem_trywait - lock a semaphore */

	/* int sem_wait(sem_t *sem);*/

	/* sem_wait()  decrements (locks) the semaphore pointed to by sem.
	 * If the semaphore's value is greater than zero, then the decrement proceeds,
	 * and the function returns,  immediately.*/
	 /* If the  semaphore  currently  has  the value zero, then the call blocks until
	  * either it becomes possible to perform the decrement (i.e., the semaphore value
	  * rises above zero), or a  signal handler interrupts the call.*/

	printf("sem_wait first time block\n");
    sem_wait(&bin_sem);
    printf("sem_wait out of block\n");
    while(strncmp("end", work_area, 3) != 0) {
        printf("You input %d characters\n", strlen(work_area) -1);
      /* If the  semaphore  currently  has  the value zero, then the call blocks until
       * either it becomes possible to perform the decrement (i.e., the semaphore value
       * rises above zero), or a  signal handler interrupts the call.*/
        sem_wait(&bin_sem);
        printf("semaphore came out of blocking state \n ");
    }
    pthread_exit(NULL);
}

/*
int sem_init(sem_t *sem, int pshared, unsigned int value);


*/
