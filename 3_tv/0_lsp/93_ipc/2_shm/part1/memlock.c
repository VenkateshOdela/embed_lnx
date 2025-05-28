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

/* shows how to use shared memory
Version : 1.0
Author : Team -C
*/

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/types.h>
# include "memkey.h"

main(){
	int shm_id,ret;
	key_t key = IPC_KEY;
	shm_id = shmget(key,8000,IPC_CREAT);
	if(shm_id <0){
		perror("shmget:");
		exit(1);
	}
	printf("shm_id = %d\n",shm_id);
	ret = shmctl(shm_id, SHM_LOCK, NULL);
	if(ret <0){
	         perror("SHMCTL LOCK :");
		 exit(1);
	}
       sleep(10);
       
	ret = shmctl(shm_id, SHM_UNLOCK, NULL);
	if(ret <0){
	         perror("SHMCTL LOCK :");
		 exit(1);
	}
	
	
}
