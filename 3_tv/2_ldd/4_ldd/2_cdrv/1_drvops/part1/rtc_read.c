/*
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * ******************************************************************************
 * This source code is part of the coursware provided with "Linux Device Drivers"
 * training program offered by Techveda <www.techveda.org>
 *
 * Copyright (C) 2020 Techveda
 *
 * Author: Raghu Bharadwaj
 *
 * Git repository:
 *   https://gitlab.com/techveda/ldd-0920
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
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "rtc.h"

int main(void)
{

	int fd, ret;
	struct rtc_time time;	
	fd = open("/dev/myrtc", O_RDWR);
	if (fd < 0) {
		perror("open call");
		return -1;
	}

	ret = read(fd, &time, sizeof(time));
	if(ret < 0){
		perror("read \n");
		exit(1);
	}
	
	printf("sec : %x\n", time.sec);
	printf("min : %x\n", time.min);
	printf("hour: %x\n", time.hour);
	printf("day : %x\n", time.day);
	printf("mon : %x\n", time.mon);
	printf("year: %x\n", time.year);

	close(fd);
	return 0;
}


