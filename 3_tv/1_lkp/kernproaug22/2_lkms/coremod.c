/*
 * ******************************************************************************
 * This code never support static linkage as no _init _exit
 *
 * Any function scope is limited to that module only.
 *
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int veda_counter;

/*
 * if you won't mention static, by default, all module functions are static
 */
int set_counter(int a)
{
	veda_counter = a;
	return 0;
}

int get_counter(void)
{
	return veda_counter;
}

/*
 * the label _init and _exit are not there , the code does not qualif
 * then myinit() is not linked into __init section
 * then myexit is not linked into __exit section
 *
 * these initializers never run for static linking;
 *
 */
static int myinit(void)
{
	printk("initializing counter\n ");
	veda_counter = 100;
	return 0;
}

static void myexit(void)
{
	printk("value of veda_counter %d\n ", veda_counter);
}

module_init(myinit);
module_exit(myexit);

/* declare function as global and can be accesssed by another module */
EXPORT_SYMBOL_GPL(set_counter);
EXPORT_SYMBOL(get_counter);

MODULE_AUTHOR ("raghu@techveda.org");
MODULE_DESCRIPTION ("LKD_CW: exporting symbols");
MODULE_LICENSE ("Dual MIT/GPL");

/*
 * EXPORT : (symbol function)
 *
 * 1.All data and Symbols defined in a modules are by default local scope
 * 2.Symbols can be selectively exported into global table using a family of Macro Called EXPORT.
 *
 * --EXPORT_SYMBOL_GPL : conditional, accessed by only GPL License modules
 *
 * --EXPORT_SYMBOL : un-conditional,
 *
 * Where we use EXPORT?
 * --
 *
 *
 * all functions of exported symbols information
 * $cat  /proc/kallsyms
 *
 * user@ubuntu:~/git_embd_linux/embd_linux/tv/lkp/kernproaug22/lkms$ cat /proc/kallsyms | grep set_counter
0000000000000000 t mci_reset_counters_store
0000000000000000 d dev_attr_reset_counters
0000000000000000 r __ksymtab_set_counter	[coremod]
0000000000000000 r __kstrtab_set_counter	[coremod]
0000000000000000 t set_counter	[coremod]

user@ubuntu:~/git_embd_linux/embd_linux/tv/lkp/kernproaug22/lkms$ cat /proc/kallsyms | grep get_counter
0000000000000000 t md_bitmap_get_counter
0000000000000000 r __ksymtab_get_counter	[coremod]
0000000000000000 r __kstrtab_get_counter	[coremod]
0000000000000000 T get_counter	[coremod]
 *
 *
 * we can also use EXPORT, to export global data variables;
 *
 * coremod.c -> coremod.0
 * coremod.mod.c -> coremod.mod.o
 *
 */

