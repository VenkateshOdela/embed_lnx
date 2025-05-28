/*
 * ******************************************************************************
 * This program is part of the source code provided with "Linux Device Drivers"
 * Course offered by Techveda <www.techveda.org>
 *
 * (C) 2020 Raghu Bharadwaj/Techveda
 *
 * Git repository:
 *   https://gitlab.com/techveda/ldd
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
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/printk.h>
#include <linux/mm.h>

static int user_pid = 1;
module_param(user_pid, int, 0);

static void print_vma(struct task_struct *task)
{
        struct mm_struct *mm;
        struct vm_area_struct *vma;
        unsigned long count = 0;

        mm = task->mm;
        pr_info("\nThis mm_struct has %d vmas.\n", mm->map_count);

	vma = mm->mmap;
	while (vma!= NULL) {
                pr_info("starts at 0x%lx, Ends at 0x%lx\n", vma->vm_start,vma->vm_end);
                count += vma->vm_end - vma->vm_start;
                vma = vma->vm_next;
        }

}

static int vm_init(void){
        struct task_struct *task;
        pr_info("\nGot the process id to look up as %d.\n", user_pid);
        for_each_process(task) {
                if ( task->pid == user_pid) {
                        pr_info("%s[%d]\n", task->comm, task->pid);
                        print_vma(task);
                }
        }
        return 0;
}

static void vm_exit(void)
{
        pr_info("\nPrint segment information module exiting.\n");
}

module_init(vm_init);
module_exit(vm_exit);


MODULE_AUTHOR ("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION ("Print segment information");
MODULE_LICENSE("Dual MIT/GPL");
