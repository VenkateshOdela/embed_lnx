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

# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>

# define KEY 8979

typedef struct{
	long msgtype;
	char buffer[100];
}msg;
int main(){
	int qid;
	int i;
	msg m1,m2;

	qid = msgget(KEY,IPC_CREAT);

	i = msgrcv(qid,&m1,sizeof(msg),20,0);
	printf("val returned %d\n", i);
	m1.buffer[99] = '\0';
	printf("%s\n",m1.buffer);


	i = msgrcv(qid,&m2,sizeof(msg),10,0);
	m2.buffer[99] = '\0';
	printf("%s\n",m2.buffer);
	return 0;
}
