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

/* test application to show how to load shared objects as runtime libs */

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	void *ptr,*ptr1;
	void (*fptr) (void);
	int ret = 0;

	printf("%s:process registered with pid %d\n", __func__, getpid());

	ptr1 = dlopen("./liba.so", RTLD_NOW | RTLD_GLOBAL);
	if (!ptr1) {
		printf("%s\n", dlerror());
		goto error;
	}

	ptr = dlopen("./libb.so", RTLD_LAZY);
	if (!ptr) {
		printf("%s\n", dlerror());
		goto error;
	}

	fptr = dlsym(ptr, "x");
	if (!fptr) {
		printf("%s\n", dlerror());
		goto error;
	}
	(fptr) ();

	/* find address of a symbol */
	fptr = dlsym(ptr, "y");
	if (!fptr) {
		printf("%s\n", dlerror());
		goto error;
	}
	(fptr) ();		/* indirect call */

	/* detach lib from process address space */
	dlclose(ptr);
	printf("%s:lib detached\n", __func__);
	return 0;

 error:
	exit(1);

}
