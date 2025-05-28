/*
 *   -- Mutual Exclusion APIs :
         -- Polling - Spin Lock
         -- Waitlock - Mutex

 *
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/slab.h>

typedef struct {
	int a;
	int b;
} priv_data;

static priv_data *p;
struct task_struct *t1, *t2;

DEFINE_MUTEX(lock); // after defining lock, default mutex is in unlock state

int kthr_reader(void *arg)
{
	pr_info("%s: attempting to lock \n",__func__);
	mutex_lock(&lock); /* Decrement atomic_t lock counter */ // kthr_reader() goes into un-interruptible wait state cannot be interrupted, won't respond to signals
	pr_info("%s: read a = %d, b = %d\n", __func__, p->a, p->b);
	mutex_unlock(&lock);

	do_exit(0);
}

int kthr_writer(void *arg)
{
	mutex_lock_interruptible(&lock); // mutex_lock_interruptible() kthr_writer goes into interruptible wait state can be interrupted through signals(kill).
	p->a = 10;
	ssleep(10);
	p->b = 20;
	mutex_unlock(&lock);

	do_exit(0);
}

void data_init(priv_data * data)
{
	data->a = 0;
	data->b = 0;
}

int kthr_init(void)
{
	p = (priv_data *) kmalloc(sizeof(priv_data), GFP_KERNEL);

	data_init(p);

	t1 = kthread_run(kthr_writer, NULL, "Kwriter");
	if(IS_ERR(t1)){
                pr_err("%s: unable to start kernel thread\n",__func__);
                return PTR_ERR(t1);
        }

	t2 = kthread_run(kthr_reader, NULL, "Kreader");
	if(IS_ERR(t2)){
                pr_err("%s: unable to start kernel thread\n",__func__);
                return PTR_ERR(t2);
        }

	return 0;
}

void kthr_exit(void)
{
	kfree(p);
}

module_init(kthr_init);
module_exit(kthr_exit);

MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("Kernel Mutex Interface");
MODULE_LICENSE("Dual MIT/GPL");



/* kernel locking 
 *
 */

/*
 * mutex_lock(struct mutex *)
 * mutex_lock_interruptible(struct mutex *)
 * mutex_lock_killable(struct mutex *) -- when it is wait state, responds only to kill signal;
 * mutex_trylock(struct mutex *) -- non blocking call; it won't put caller to wait state. if lock present return 0, if lock not present returns -1
 *
 *
 */

/* process state :
 * wait, run , ready, stop
 *
 * wait two types :
 *  1. interruptible wait
 *  2. uninterruptible wait
 *
 *
 */
