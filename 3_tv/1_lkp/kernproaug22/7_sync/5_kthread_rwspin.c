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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/sched.h>

#define MODNAME "[SYNC_RWSPINLOCK]: "

/* shared data: */
unsigned int counter;

DEFINE_RWLOCK(counter_lock);

struct task_struct *read_thread1, *read_thread2, *read_thread3, *write_thread;

static int writer_function(void *data)
{
	while(!kthread_should_stop()) {
		write_lock(&counter_lock);
		counter++;
		write_unlock(&counter_lock);
		msleep(500);
	}
	do_exit(0);
}

static int read_function(void *data)
{
	while(!kthread_should_stop()) {
		read_lock(&counter_lock);
		pr_info("%s:counter: %d\n",__func__, counter);
		read_unlock(&counter_lock);
		msleep(500);
	}
	do_exit(0);
}

static int __init my_mod_init(void)
{
	pr_info(MODNAME "Entering module.\n");
	counter = 0;
	read_thread1 = kthread_run(read_function, NULL, "read-thread1");
	read_thread2 = kthread_run(read_function, NULL, "read-thread2");
	read_thread3 = kthread_run(read_function, NULL, "read-thread3");
	write_thread = kthread_run(writer_function, NULL, "write-thread");
	return 0;
}


static void __exit my_mod_exit(void)
{
	kthread_stop(read_thread3);
	kthread_stop(read_thread2);
	kthread_stop(read_thread1);
	kthread_stop(write_thread);
	pr_info(MODNAME"Exiting module.\n");
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("rwlocks");
MODULE_LICENSE("Dual MIT/GPL");
