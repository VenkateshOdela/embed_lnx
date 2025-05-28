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


# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/sem.h>

# define KEY 1234

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};

main(){
	int semid,retval;
	struct sembuf *sbuf;
	union semun su,su1;
	sbuf = (struct sembuf*) malloc(sizeof(struct sembuf));
	semid = semget(KEY,1,IPC_CREAT);
	printf(" semid = %d\n",semid);
	sbuf->sem_num = 0;
	sbuf->sem_op = 1; // add this value to semaphore.
	sbuf->sem_flg = SEM_UNDO; // IPC_NOWAIT can be used.
	semop(semid,sbuf,1);
	printf(" semval set to 1\n");
	getchar();
	getchar();
	su.val=0;
	retval = semctl(semid,0,SETVAL,su);
	sbuf->sem_num = 0;
	sbuf->sem_op = 0; // add this value to semaphore.
	sbuf->sem_flg = IPC_NOWAIT; // IPC_NOWAIT can be used.
	printf("before setting semval to 0\n");
	retval = semop(semid,sbuf,1);
	printf("After setting semval to 0  retval = %d\n",retval);
	perror("semval:");
	getchar();
	getchar();
}
