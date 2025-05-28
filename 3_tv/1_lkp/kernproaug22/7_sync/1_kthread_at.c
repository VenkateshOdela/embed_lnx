/*
 *
 * A kernel thread creation using kthread_run = kthread_create + wake_up_process() ;
 *
 * two kernel threads are created :
 * 1. read_counter thread
 * 2. write_counter thread
 *
 * As two threads operating separately no possibility of race condition;
 *  -- as one thread in read mode and other thread in write mode
 *
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/types.h>

#define MODNAME "[SYNC_ATOMIC] "

atomic_t counter; 	/* shared data: */
struct task_struct *read_thread, *write_thread;

static int writer_function(void *data)
{
	while(!kthread_should_stop()) {
		atomic_inc(&counter);
		msleep(500);
	}
	do_exit(0);
}

static int read_function(void *data)
{
	while(!kthread_should_stop()) {
		pr_info(MODNAME "counter: %d\n", atomic_read(&counter));
		msleep(500);
	}
	do_exit(0);
}

static int __init my_mod_init(void)
{
	pr_info("%s Entering module.\n", MODNAME);
	atomic_set(&counter, 0); // seting atomic_t counter to Zero
	read_thread = kthread_run(read_function, NULL, "read-thread"); // kthread_run = kthread_create + wake_up_process()
	write_thread = kthread_run(writer_function, NULL, "write-thread");
	return 0;
}

static void __exit my_mod_exit(void)
{
	kthread_stop(read_thread);
	kthread_stop(write_thread);
	pr_info("Exiting module.\n");
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("atomic counters");
MODULE_LICENSE("Dual MIT/GPL");

/*
 *  sudo insmod kthread_at.ko
 *
 * user@ubuntu:~/git_embd_linux/embd_linux/tv/lkp/kernproaug22/7_sync$ dmesg
[66554.745450] kthread_at: loading out-of-tree module taints kernel.
[66554.745811] kthread_at: module verification failed: signature and/or required key missing - tainting kernel
[66554.749048] [SYNC_ATOMIC]  Entering module.
[66554.749258] [SYNC_ATOMIC] counter: 0
[66555.253948] [SYNC_ATOMIC] counter: 2
[66555.762765] [SYNC_ATOMIC] counter: 2
[66556.272901] [SYNC_ATOMIC] counter: 3
[66556.781948] [SYNC_ATOMIC] counter: 5
[66557.291347] [SYNC_ATOMIC] counter: 6
[66557.800347] [SYNC_ATOMIC] counter: 7
[66558.309575] [SYNC_ATOMIC] counter: 7
[66558.818561] [SYNC_ATOMIC] counter: 8
[66559.328006] [SYNC_ATOMIC] counter: 9
[66559.837270] [SYNC_ATOMIC] counter: 11
[66560.347028] [SYNC_ATOMIC] counter: 11
[66560.855870] [SYNC_ATOMIC] counter: 13
[66561.365237] [SYNC_ATOMIC] counter: 13
[66561.874927] [SYNC_ATOMIC] counter: 15
[66562.384491] [SYNC_ATOMIC] counter: 16
[66562.893153] [SYNC_ATOMIC] counter: 17
[66563.409780] [SYNC_ATOMIC] counter: 17
 *
 *
 * sudo rmmod kthread_at
 * exiting module
 *
 *
 *
 */
