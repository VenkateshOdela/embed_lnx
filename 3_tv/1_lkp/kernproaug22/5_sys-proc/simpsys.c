/*
 *
 *
 *
 */

#include <linux/module.h>
#include <linux/printk.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>

#define MODNAME "[simpsys]"

static struct kobject *example_kobject;
static int foo;

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
	dump_stack();
        return sprintf(buf, "%d\n", foo);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
                      const char *buf, size_t count)
{
	dump_stack();
        sscanf(buf, "%du", &foo);
        return count;
}


static struct kobj_attribute foo_attribute =__ATTR(foo, 0660, foo_show,
                                                   foo_store);

static int __init simple_init (void)
{
        int error = 0;

        pr_debug("Module initialized successfully \n");

        example_kobject = kobject_create_and_add("kobject_example",
                                                 kernel_kobj);
        if(!example_kobject)
                return -ENOMEM;

        error = sysfs_create_file(example_kobject, &foo_attribute.attr);
        if (error) {
                pr_debug("failed to create the foo file in /sys/kernel/kobject_example \n");
        }

        return error;
}

static void __exit simple_exit (void)
{
        pr_debug ("Module un initialized successfully \n");
        kobject_put(example_kobject);
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_DESCRIPTION("simple sys file");
MODULE_LICENSE("Dual BSD/GPL");


/*
 *
 * user@ubuntu:/sys/kernel$ ls | grep kobject_example
   kobject_example


  super user permissions are required for writing to file /kobject_example/foo

  root@ubuntu:/sys/kernel/kobject_example# echo 10 > foo


  user@ubuntu:/sys/kernel$ dmesg
[57586.936818] CPU: 1 PID: 59803 Comm: bash Tainted: G           OE     5.4.0-128-generic #144~18.04.1-Ubuntu
[57586.936819] Hardware name: VMware, Inc. VMware Virtual Platform/440BX Desktop Reference Platform, BIOS 6.00 05/19/2017
[57586.936820] Call Trace:
[57586.937761]  dump_stack+0x6d/0x8b
[57586.937782]  foo_store+0x17/0x40 [simpsys]
[57586.938180]  kobj_attr_store+0x12/0x20
[57586.938553]  sysfs_kf_write+0x3c/0x50
[57586.938556]  kernfs_fop_write+0x13e/0x1c0
[57586.938810]  __vfs_write+0x1b/0x40
[57586.938813]  vfs_write+0xb1/0x1a0
[57586.938813]  ksys_write+0xa7/0xe0
[57586.938814]  __x64_sys_write+0x1a/0x20
[57586.938877]  do_syscall_64+0x57/0x190
[57586.938880]  entry_SYSCALL_64_after_hwframe+0x44/0xa9
[57586.938881] RIP: 0033:0x7f2fcc80e104
[57586.938883] Code: 89 02 48 c7 c0 ff ff ff ff c3 66 2e 0f 1f 84 00 00 00 00 00 66 90 48 8d 05 e1 08 2e 00 8b 00 85 c0 75 13 b8 01 00 00 00 0f 05 <48> 3d 00 f0 ff ff 77 54 f3 c3 66 90 41 54 55 49 89 d4 53 48 89 f5
[57586.938883] RSP: 002b:00007ffedd87d8c8 EFLAGS: 00000246 ORIG_RAX: 0000000000000001
[57586.938897] RAX: ffffffffffffffda RBX: 0000000000000003 RCX: 00007f2fcc80e104
[57586.938897] RDX: 0000000000000003 RSI: 000055b9def9b4e0 RDI: 0000000000000001
[57586.938897] RBP: 000055b9def9b4e0 R08: 000000000000000a R09: 0000000000000002
[57586.938898] R10: 000000000000000a R11: 0000000000000246 R12: 00007f2fccaea760
[57586.938898] R13: 0000000000000003 R14: 00007f2fccae62a0 R15: 00007f2fccae5760


  root@ubuntu:/sys/kernel/kobject_example# cat foo
10

  user@ubuntu:/sys/kernel$ dmesg
[57912.166527] CPU: 0 PID: 59855 Comm: cat Tainted: G           OE     5.4.0-128-generic #144~18.04.1-Ubuntu
[57912.166527] Hardware name: VMware, Inc. VMware Virtual Platform/440BX Desktop Reference Platform, BIOS 6.00 05/19/2017
[57912.166528] Call Trace:
[57912.166532]  dump_stack+0x6d/0x8b
[57912.166535]  foo_show+0x12/0x2c [simpsys]
[57912.166537]  kobj_attr_show+0x12/0x20
[57912.166538]  sysfs_kf_seq_show+0xa1/0x110
[57912.166539]  kernfs_seq_show+0x27/0x30
[57912.166574]  seq_read+0xda/0x440
[57912.166576]  kernfs_fop_read+0x141/0x1a0
[57912.166577]  __vfs_read+0x1b/0x40
[57912.166578]  vfs_read+0x8e/0x130
[57912.166578]  ksys_read+0xa7/0xe0
[57912.166579]  __x64_sys_read+0x1a/0x20
[57912.166593]  do_syscall_64+0x57/0x190
[57912.166595]  entry_SYSCALL_64_after_hwframe+0x44/0xa9
[57912.166596] RIP: 0033:0x7f25166a9031
[57912.166597] Code: fe ff ff 48 8d 3d cf 9b 0a 00 48 83 ec 08 e8 56 4b 02 00 66 0f 1f 44 00 00 48 8d 05 b1 09 2e 00 8b 00 85 c0 75 13 31 c0 0f 05 <48> 3d 00 f0 ff ff 77 57 f3 c3 0f 1f 44 00 00 41 54 55 49 89 d4 53
[57912.166598] RSP: 002b:00007ffd052e7c38 EFLAGS: 00000246 ORIG_RAX: 0000000000000000
[57912.166599] RAX: ffffffffffffffda RBX: 0000000000020000 RCX: 00007f25166a9031
[57912.166599] RDX: 0000000000020000 RSI: 00007f2516b6f000 RDI: 0000000000000003
[57912.166600] RBP: 0000000000020000 R08: 00000000ffffffff R09: 0000000000000000
[57912.166600] R10: 0000000000000022 R11: 0000000000000246 R12: 00007f2516b6f000
[57912.166600] R13: 0000000000000003 R14: 00007f2516b6f00f R15: 0000000000020000




 */
