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

# include <stdio.h>
# include <dlfcn.h>
# include <stdlib.h>

int main()
{

	char *ptr;
	void *handle;
 
	/* step 1: Declare function pointer */
	void (*fnptr)(void);
	printf("%s: start of main\n",__func__);
	getchar();

/* step 2: request link-loader to load specified library and 
           attach it into our address space */

	handle = dlopen("./libinit.so", RTLD_NOW);
	if(handle == NULL){
		printf(" Failed to load library \n");
		exit(2);
	}
/* step 3: lookup for the address of the required function */

	fnptr = dlsym(handle,"f1");
	getchar();

/* step 4: invoke function through  pointer */
	(fnptr)();// indirect call

/* step 5: request link-loader to unlink library */
	dlclose(handle);

	printf("%s: end of main\n",__func__);
	getchar();
	return 0;
}	
