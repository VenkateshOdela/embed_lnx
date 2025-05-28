/*
 * ******************************************************************************
 * This program is part of the source code provided with "Linux Kernel Programming"
 * Course offered by Techveda <www.techveda.org>
 *
 * (C) 2020 Raghu Bharadwaj/Techveda
 *
 * Git repository:
 *   https://gitlab.com/techveda/kernpro
 * ******************************************************************************
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
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
 *
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

/* extern is used to tell compiler that these are defines else where */
extern void set_counter (int);
extern int get_counter (void);

/*
static int func(void)
{
	int a;
	set_counter(200);
	a = get_counter();
	printk("counter set to %d\n", a);
	return 0;
}*/

int init_module (void)
{
	int a = 0;
	set_counter(200);
	a = get_counter();
	printk("counter set to %d\n", a);

	return 0;
}

void cleanup_module (void)
{
}

MODULE_AUTHOR ("raghu@techveda.org");
MODULE_DESCRIPTION ("LKD_CW : accessing exported symbols");
MODULE_LICENSE ("GPL v2");
//MODULE_LICENSE ("");

/*
 * $sudo insmod usermod.ko
 *
 * $lsmod
 *
 * Module                  Size  Used by
usermod                16384  0
coremod                16384  1 usermod
 *
 * Note : Ref Count of coremod is now incremented by 1
 *        usermod is dependent on coremod
 *
 *Note : Ref count is more but no modules not listed, it means they are used static kernel
 *
 *
 *we cannot remove coremod when usermod is running :
 *user@ubuntu:~/git_embd_linux/embd_linux/tv/lkp/kernproaug22/lkms$ sudo rmmod coremod
   rmmod: ERROR: Module coremod is in use by: usermod
 *
 *user@ubuntu:~/git_embd_linux/embd_linux/tv/lkp/kernproaug22/lkms$ cat Module.symvers
0xb5ffd3b8	get_counter	/home/user/git_embd_linux/embd_linux/tv/lkp/kernproaug22/lkms/coremod	EXPORT_SYMBOL
0x3c43898a	set_counter	/home/user/git_embd_linux/embd_linux/tv/lkp/kernproaug22/lkms/coremod	EXPORT_SYMBOL_GPL
 *
 *
 * Module.symvers : (file)Export Symbol information is stored in this file, it is generated in specific directory of
 *                       of that module;
 *
 * printk also Export Symbol :
 *  how to verify this ?
 *
 *   user@ubuntu:/lib/modules/5.4.0-124-generic/build$ cat Module.symvers | grep printk
 *   0xc5850110	printk	vmlinux	EXPORT_SYMBOL
 *
 * all linux kernel export symbols are present in below file :
 * /lib/modules/5.4.0-124-generic/build/Module.symvers
 *
 *
 */
