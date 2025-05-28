/*
 * a simple kernel module
 *
 */

 /* kernel headers : from kernel sources */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>

/* module body */
/* module can be driver, fs
 * modules are not drivers;
 * drivers are loaded through modules;
 *  */
int func(void)
{
	printk("Hello module function\n");
	//ssleep(60);
	return 0;
}
/*
 * initilizer function
 * name : init_module
 *
 * if init function is exited with negative number return -1, module linkage will not happen
 * if init function is exited with return 0,module linkage will happen;
 *
 * consider module body as device driver,
 * in init function,check for device existance and then only load device driver, else don't
 * (checking device is physically present or not)
 * this return -1 is use case for loading device driver only if device physically exist.
 */
int init_module(void)
{

	printk("Hello Venkatesh\n");
	func();
    //ssleep(60); // secondssleep
	//dump_stack(); // get kernel stack dump of insmod process; can be seen kernel ring buffer(dmesg);see bottom to top;
	return 0;
}
/*
 * exit function
 * name : cleanup_module
 */

void cleanup_module(void)
{
	printk("Goodbye Venkatesh\n");
	//dump_stack(); // get kernel stack dump of rmmod process; can be seen kernel ring buffer(dmesg);see bottom to top;
	//sseconds(30);
}

/* comment macros */
/* These are placed in modinfo SECTION */
MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("raghu@techveda.org");
MODULE_DESCRIPTION("First kernel module");

/*
 * a module is identical to shared object
 * linker ld to create shared object.
 * extending address_space
 * the shared object exchange code and data, init time or run time.
 * .ko - kernel module
 * how shared object are handled is applies to ko;
 *
 * Out of Tree kernel module
 * -source code of this kernel modules is out of kernel source tree.
 *
 * scripts directory in kernel source tree :
 * Makefile.modinst
 * Makefile.modpost
 * Makefile.modsign
 *
 * when you install linux distribution ubuntu, kernel sources are not present.
 *
 * root file system of all distributions :
 *
 * $/lib/modules
 *
 * user@ubuntu:/lib/modules$ ls
5.4.0-100-generic  5.4.0-107-generic  5.4.0-113-generic  5.4.0-121-generic  5.4.0-125-generic  5.4.0-91-generic  5.4.0-96-generic
5.4.0-104-generic  5.4.0-109-generic  5.4.0-117-generic  5.4.0-122-generic  5.4.0-84-generic   5.4.0-92-generic  5.4.0-99-generic
5.4.0-105-generic  5.4.0-110-generic  5.4.0-120-generic  5.4.0-124-generic  5.4.0-90-generic   5.4.0-94-generic
user@ubuntu:/lib/modules$
 *
 * as ubuntu update these files update with new generic
 * user@ubuntu:/lib/modules$ uname -r
    5.4.0-124-generic
 *
 *  in  tree kernel module
 *
 *  user@ubuntu:/lib/modules/5.4.0-124-generic/kernel$ ls
arch  block  crypto  drivers  fs  kernel  lib  mm  net  samples  sound  ubuntu  virtualbox-guest  wireguard  zfs
 *
 * require libraries for compilation : to generate out of tree module
 *  /lib/modules/5.4.0-124-generic/build
 *
 * user@ubuntu:/lib/modules/5.4.0-124-generic/build$ ls
arch   certs   Documentation  fs       init  Kbuild   kernel  Makefile  Module.symvers  samples  security  tools   usr
block  crypto  drivers        include  ipc   Kconfig  lib     mm        net             scripts  sound     ubuntu  virt
 *
 *
 *
 *
 *
 *
 *
 */
