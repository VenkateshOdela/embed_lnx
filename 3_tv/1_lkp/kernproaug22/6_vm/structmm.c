/*
 *
 *
 *
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/current.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/delay.h>

int init_module(void)
{
	struct task_struct *tsk = current; // get address of task_struct using current macro
        struct mm_struct *mm = tsk->mm; // get access to mm descriptor of the process 
        unsigned long stack_size = (mm->stack_vm << PAGE_SHIFT); // print stack size
        unsigned long down_to = mm->start_stack - stack_size;// print stack end address 

        pr_info( "Info from the Memory Management structure " );
        pr_info(   "for task \'%s\' ", tsk->comm );
        pr_info( "(pid=%d) ", tsk->pid );
        pr_info( "   pgd=%08lX  ", (unsigned long)mm->pgd );
        pr_info( "mmap=%08lX  ", (unsigned long)mm->mmap );
        pr_info( "map_count=%d  ", mm->map_count );
        pr_info( "mm_users=%d  ", mm->mm_users.counter );
        pr_info( "mm_count=%d  ", mm->mm_count.counter );
        pr_info( "\n" );
        pr_info( "    start_code=%08lX  ", mm->start_code );
        pr_info( " end_code=%08lX\n", mm->end_code );
        pr_info( "    start_data=%08lX  ", mm->start_data );
        pr_info( " end_data=%08lX\n", mm->end_data );
        pr_info( "     start_brk=%08lX  ", mm->start_brk );
        pr_info( "      brk=%08lX\n", mm->brk );
        pr_info( "     arg_start=%08lX  ", mm->arg_start );
        pr_info( "  arg_end=%08lX\n", mm->arg_end );
        pr_info( "     env_start=%08lX  ", mm->env_start );
        pr_info( "  env_end=%08lX\n", mm->env_end );
        pr_info( "   start_stack=%08lX  ", mm->start_stack );
        pr_info( "  down_to=%08lX ", down_to );
        pr_info( " <--- stack grows downward " );

	return -EINVAL;
}

void cleanup_module(void)
{
	pr_info("Unloading");
}

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("Memory descriptor");


/*
 * insmod trigger init_module from user space;
 */

/*
 * user@ubuntu:~/git_embd_linux/embd_linux/tv/lkp/kernproaug22/vm$ dmesg
[15072.900270] structmm: loading out-of-tree module taints kernel.
[15072.913023] structmm: module verification failed: signature and/or required key missing - tainting kernel
[15072.932021] Info from the Memory Management structure
[15072.932023] for task 'insmod'
[15072.932026] (pid=5818)
[15072.932027]    pgd=FFFF9A2F8F59A000
[15072.932027] mmap=FFFF9A2FC94B8680
[15072.932028] map_count=18
[15072.932028] mm_users=1
[15072.932029] mm_count=1

[15072.932030]     start_code=560ED1153000
[15072.932030]  end_code=560ED1175D50
[15072.932031]     start_data=560ED1375E10
[15072.932031]  end_data=560ED1377080
[15072.932032]      start_brk=560ED2DCC000
[15072.932032]       brk=560ED2DED000
[15072.932032]      arg_start=7FFE6C6E168E
[15072.932033]   arg_end=7FFE6C6E16A1
[15072.932033]      env_start=7FFE6C6E16A1
[15072.932033]   env_end=7FFE6C6E1FEB
[15072.932034]    start_stack=7FFE6C6E0E80
[15072.932034]   down_to=7FFE6C6BFE80
 *
 */

/*
 * cat /proc/<pid>/maps
 *
 *
 */
