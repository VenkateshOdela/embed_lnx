
/*
 *   - sudo mknod /dev/abc c 190 0
 *
 *   - /dev/abc
 *   - device number registration.
 *   - char device registration in vfs
 *   - perform file operations from user space by open(/dev/abc)
 *
 *   - make
 *   - sudo inmsod 1_cdrv.c
 *
 *   - driver name charrdrvv
 *   - sudo inmsod 1_cdrv.ko
 *	 - see /proc/devices is device number allotted.
 *	 	- 190 charrdrvv
 *	 - see dmesg
 *	 	- [102721.967814] Driver Registered charrdrvv
 *
 *	 - lsmod is for only modules, use /proc/devices to see registered drivers
 *
 *
 */



#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define MAJORNO 190
#define MINORNO  0
#define DRV_NAME "chardrv"
#define COUNT 1
#define SUCCESS 0

static struct cdev *veda_cdev;
static dev_t mydev;

static int cdrv_open(struct inode *inode, struct file *file)
{
	dump_stack(); // call trace in dmesg
	pr_info("%s() invoked \n", __func__);
	return SUCCESS;
}

static int cdrv_release(struct inode *inode, struct file *file)
{
	dump_stack();
	pr_info("%s() invoked \n", __func__);
	return SUCCESS;
}

static ssize_t cdrv_write(struct file *file, const char __user
			  * buf, size_t lbuf, loff_t * offset)
{
	dump_stack();
	pr_info("%s() invoked:Rec'vd data of len = %ld\n", __func__,
		lbuf);
	return lbuf;
}

static ssize_t cdrv_read(struct file *file, char __user * buf,
			 size_t count, loff_t * off)
{
	dump_stack();
	pr_info("%s() invoked.\n"
		"request to read %zu bytes\n", __func__, count);
	return count;
}

// vfs define structure file operations
// this file operation function holds the function pointers
// bind cdrv_fops object with inode already created with mknod
static struct file_operations cdrv_fops = {
	.owner = THIS_MODULE,
	.write = cdrv_write,
	.read = cdrv_read,
	.open = cdrv_open,
	.release = cdrv_release
};

static __init int cdrv_init(void)
{
	int ret;

	/* step 1: claim ownership of the device file identified by devid */
	mydev = MKDEV(MAJORNO, MINORNO);
	register_chrdev_region(mydev, COUNT, DRV_NAME);

	/* step 2: Register driver operations with VFS  
	   Allocate cdev instance */
	/* cdev_alloc() returns instance of cdev_instance */
	veda_cdev = cdev_alloc();

	/* initialize cdev with fops object */
	cdev_init(veda_cdev, &cdrv_fops);

	/* register cdev */
	ret = cdev_add(veda_cdev, mydev, COUNT);
	if (ret < 0) {
		pr_err("Error registering device driver");
		return ret;
	}
	pr_info("Driver Registered %s\n", DRV_NAME);
	return SUCCESS;
}

static __exit void cdrv_remove(void)
{
	/* remove cdev */
	cdev_del(veda_cdev);

	/* free major/minor no's used */
	unregister_chrdev_region(mydev, COUNT);
	pr_info("%s: Device detached\n", __func__);
}

module_init(cdrv_init);
module_exit(cdrv_remove);

MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("char driver skeleton");
MODULE_LICENSE("Dual MIT/GPL");


/*
 * user@ubuntu:~$ dmesg
[103181.046208] CPU: 2 PID: 83632 Comm: a.out Tainted: G           OE     5.4.0-135-generic #152~18.04.2-Ubuntu
[103181.046209] Hardware name: VMware, Inc. VMware Virtual Platform/440BX Desktop Reference Platform, BIOS 6.00 05/19/2017
[103181.046209] Call Trace:
[103181.046214]  dump_stack+0x6d/0x8b
[103181.046216]  cdrv_open+0xe/0x30 [1_cdrv]
[103181.046218]  chrdev_open+0xc4/0x1b0
[103181.046219]  ? cdev_put.part.3+0x20/0x20
[103181.046220]  do_dentry_open+0x1df/0x3a0
[103181.046221]  vfs_open+0x2f/0x40
[103181.046222]  path_openat+0x2f9/0x16a0
[103181.046224]  ? unlock_page_memcg+0x12/0x20
[103181.046225]  ? page_add_file_rmap+0x13a/0x180
[103181.046226]  ? filemap_map_pages+0x181/0x3b0
[103181.046227]  do_filp_open+0x9b/0x110
[103181.046228]  ? __check_object_size+0xdb/0x1b0
[103181.046230]  ? __alloc_fd+0x46/0x170
[103181.046231]  do_sys_open+0x1ba/0x2e0
[103181.046231]  ? do_sys_open+0x1ba/0x2e0
[103181.046233]  ? __do_page_fault+0x2be/0x4d0
[103181.046234]  __x64_sys_openat+0x20/0x30
[103181.046235]  do_syscall_64+0x57/0x190
[103181.046236]  entry_SYSCALL_64_after_hwframe+0x5c/0xc1
[103181.046237] RIP: 0033:0x7f005cdb3c3e
[103181.046238] Code: 25 00 00 41 00 3d 00 00 41 00 74 48 48 8d 05 b1 0d 2e 00 8b 00 85 c0 75 69 89 f2 b8 01 01 00 00 48 89 fe bf 9c ff ff ff 0f 05 <48> 3d 00 f0 ff ff 0f 87 a6 00 00 00 48 8b 4c 24 28 64 48 33 0c 25
[103181.046239] RSP: 002b:00007ffc31b6a3a0 EFLAGS: 00000246 ORIG_RAX: 0000000000000101
[103181.046240] RAX: ffffffffffffffda RBX: 0000000000000000 RCX: 00007f005cdb3c3e
[103181.046240] RDX: 0000000000000002 RSI: 00005562945e79e8 RDI: 00000000ffffff9c
[103181.046240] RBP: 00007ffc31b6b430 R08: 00007f005d090d80 R09: 00007f005d090d80
[103181.046241] R10: 0000000000000000 R11: 0000000000000246 R12: 00005562945e7710
[103181.046241] R13: 00007ffc31b6b510 R14: 0000000000000000 R15: 0000000000000000
[103181.046242] cdrv_open() invoked

[103245.989382] CPU: 3 PID: 83632 Comm: a.out Tainted: G           OE     5.4.0-135-generic #152~18.04.2-Ubuntu
[103245.989383] Hardware name: VMware, Inc. VMware Virtual Platform/440BX Desktop Reference Platform, BIOS 6.00 05/19/2017
[103245.989383] Call Trace:
[103245.989388]  dump_stack+0x6d/0x8b
[103245.989391]  cdrv_write+0x12/0x30 [1_cdrv]
[103245.989393]  __vfs_write+0x1b/0x40
[103245.989394]  vfs_write+0xb1/0x1a0
[103245.989394]  ksys_write+0xa7/0xe0
[103245.989395]  __x64_sys_write+0x1a/0x20
[103245.989397]  do_syscall_64+0x57/0x190
[103245.989398]  entry_SYSCALL_64_after_hwframe+0x5c/0xc1
[103245.989399] RIP: 0033:0x7f005cdb4104
[103245.989400] Code: 89 02 48 c7 c0 ff ff ff ff c3 66 2e 0f 1f 84 00 00 00 00 00 66 90 48 8d 05 e1 08 2e 00 8b 00 85 c0 75 13 b8 01 00 00 00 0f 05 <48> 3d 00 f0 ff ff 77 54 f3 c3 66 90 41 54 55 49 89 d4 53 48 89 f5
[103245.989401] RSP: 002b:00007ffc31b6a408 EFLAGS: 00000246 ORIG_RAX: 0000000000000001
[103245.989402] RAX: ffffffffffffffda RBX: 0000000000000000 RCX: 00007f005cdb4104
[103245.989402] RDX: 000000000000000c RSI: 00007ffc31b6a420 RDI: 0000000000000003
[103245.989403] RBP: 00007ffc31b6b430 R08: 00007f005d0918c0 R09: 00007f005d29a4c0
[103245.989428] R10: 0000556296312010 R11: 0000000000000246 R12: 00005562945e7710
[103245.989429] R13: 00007ffc31b6b510 R14: 0000000000000000 R15: 0000000000000000
[103245.989430] cdrv_write() invoked:Rec'vd data of len = 12
user@ubuntu:~$ dmesg
[103181.046208] CPU: 2 PID: 83632 Comm: a.out Tainted: G           OE     5.4.0-135-generic #152~18.04.2-Ubuntu
[103181.046209] Hardware name: VMware, Inc. VMware Virtual Platform/440BX Desktop Reference Platform, BIOS 6.00 05/19/2017
[103181.046209] Call Trace:
[103181.046214]  dump_stack+0x6d/0x8b
[103181.046216]  cdrv_open+0xe/0x30 [1_cdrv]
[103181.046218]  chrdev_open+0xc4/0x1b0
[103181.046219]  ? cdev_put.part.3+0x20/0x20
[103181.046220]  do_dentry_open+0x1df/0x3a0
[103181.046221]  vfs_open+0x2f/0x40
[103181.046222]  path_openat+0x2f9/0x16a0
[103181.046224]  ? unlock_page_memcg+0x12/0x20
[103181.046225]  ? page_add_file_rmap+0x13a/0x180
[103181.046226]  ? filemap_map_pages+0x181/0x3b0
[103181.046227]  do_filp_open+0x9b/0x110
[103181.046228]  ? __check_object_size+0xdb/0x1b0
[103181.046230]  ? __alloc_fd+0x46/0x170
[103181.046231]  do_sys_open+0x1ba/0x2e0
[103181.046231]  ? do_sys_open+0x1ba/0x2e0
[103181.046233]  ? __do_page_fault+0x2be/0x4d0
[103181.046234]  __x64_sys_openat+0x20/0x30
[103181.046235]  do_syscall_64+0x57/0x190
[103181.046236]  entry_SYSCALL_64_after_hwframe+0x5c/0xc1
[103181.046237] RIP: 0033:0x7f005cdb3c3e
[103181.046238] Code: 25 00 00 41 00 3d 00 00 41 00 74 48 48 8d 05 b1 0d 2e 00 8b 00 85 c0 75 69 89 f2 b8 01 01 00 00 48 89 fe bf 9c ff ff ff 0f 05 <48> 3d 00 f0 ff ff 0f 87 a6 00 00 00 48 8b 4c 24 28 64 48 33 0c 25
[103181.046239] RSP: 002b:00007ffc31b6a3a0 EFLAGS: 00000246 ORIG_RAX: 0000000000000101
[103181.046240] RAX: ffffffffffffffda RBX: 0000000000000000 RCX: 00007f005cdb3c3e
[103181.046240] RDX: 0000000000000002 RSI: 00005562945e79e8 RDI: 00000000ffffff9c
[103181.046240] RBP: 00007ffc31b6b430 R08: 00007f005d090d80 R09: 00007f005d090d80
[103181.046241] R10: 0000000000000000 R11: 0000000000000246 R12: 00005562945e7710
[103181.046241] R13: 00007ffc31b6b510 R14: 0000000000000000 R15: 0000000000000000
[103181.046242] cdrv_open() invoked
[103245.989382] CPU: 3 PID: 83632 Comm: a.out Tainted: G           OE     5.4.0-135-generic #152~18.04.2-Ubuntu
[103245.989383] Hardware name: VMware, Inc. VMware Virtual Platform/440BX Desktop Reference Platform, BIOS 6.00 05/19/2017
[103245.989383] Call Trace:
[103245.989388]  dump_stack+0x6d/0x8b
[103245.989391]  cdrv_write+0x12/0x30 [1_cdrv]
[103245.989393]  __vfs_write+0x1b/0x40
[103245.989394]  vfs_write+0xb1/0x1a0
[103245.989394]  ksys_write+0xa7/0xe0
[103245.989395]  __x64_sys_write+0x1a/0x20
[103245.989397]  do_syscall_64+0x57/0x190
[103245.989398]  entry_SYSCALL_64_after_hwframe+0x5c/0xc1
[103245.989399] RIP: 0033:0x7f005cdb4104
[103245.989400] Code: 89 02 48 c7 c0 ff ff ff ff c3 66 2e 0f 1f 84 00 00 00 00 00 66 90 48 8d 05 e1 08 2e 00 8b 00 85 c0 75 13 b8 01 00 00 00 0f 05 <48> 3d 00 f0 ff ff 77 54 f3 c3 66 90 41 54 55 49 89 d4 53 48 89 f5
[103245.989401] RSP: 002b:00007ffc31b6a408 EFLAGS: 00000246 ORIG_RAX: 0000000000000001
[103245.989402] RAX: ffffffffffffffda RBX: 0000000000000000 RCX: 00007f005cdb4104
[103245.989402] RDX: 000000000000000c RSI: 00007ffc31b6a420 RDI: 0000000000000003
[103245.989403] RBP: 00007ffc31b6b430 R08: 00007f005d0918c0 R09: 00007f005d29a4c0
[103245.989428] R10: 0000556296312010 R11: 0000000000000246 R12: 00005562945e7710
[103245.989429] R13: 00007ffc31b6b510 R14: 0000000000000000 R15: 0000000000000000
[103245.989430] cdrv_write() invoked:Rec'vd data of len = 12
[103276.257248] CPU: 2 PID: 83632 Comm: a.out Tainted: G           OE     5.4.0-135-generic #152~18.04.2-Ubuntu
[103276.257249] Hardware name: VMware, Inc. VMware Virtual Platform/440BX Desktop Reference Platform, BIOS 6.00 05/19/2017
[103276.257249] Call Trace:
[103276.257273]  dump_stack+0x6d/0x8b
[103276.257277]  cdrv_read+0x12/0x2e [1_cdrv]
[103276.257279]  __vfs_read+0x1b/0x40
[103276.257279]  vfs_read+0x8e/0x130
[103276.257280]  ksys_read+0xa7/0xe0
[103276.257280]  __x64_sys_read+0x1a/0x20
[103276.257282]  do_syscall_64+0x57/0x190
[103276.257284]  entry_SYSCALL_64_after_hwframe+0x5c/0xc1
[103276.257285] RIP: 0033:0x7f005cdb4031
[103276.257286] Code: fe ff ff 48 8d 3d cf 9b 0a 00 48 83 ec 08 e8 56 4b 02 00 66 0f 1f 44 00 00 48 8d 05 b1 09 2e 00 8b 00 85 c0 75 13 31 c0 0f 05 <48> 3d 00 f0 ff ff 77 57 f3 c3 0f 1f 44 00 00 41 54 55 49 89 d4 53
[103276.257287] RSP: 002b:00007ffc31b6a408 EFLAGS: 00000246 ORIG_RAX: 0000000000000000
[103276.257288] RAX: ffffffffffffffda RBX: 0000000000000000 RCX: 00007f005cdb4031
[103276.257288] RDX: 0000000000000bb8 RSI: 00007ffc31b6a420 RDI: 0000000000000003
[103276.257288] RBP: 00007ffc31b6b430 R08: 00007f005d0918c0 R09: 00007f005d29a4c0
[103276.257289] R10: 0000556296312010 R11: 0000000000000246 R12: 00005562945e7710
[103276.257289] R13: 00007ffc31b6b510 R14: 0000000000000000 R15: 0000000000000000
[103276.257290] cdrv_read() invoked.
                request to read 3000 bytes
user@ubuntu:~$ dmesg
[103181.046208] CPU: 2 PID: 83632 Comm: a.out Tainted: G           OE     5.4.0-135-generic #152~18.04.2-Ubuntu
[103181.046209] Hardware name: VMware, Inc. VMware Virtual Platform/440BX Desktop Reference Platform, BIOS 6.00 05/19/2017
[103181.046209] Call Trace:
[103181.046214]  dump_stack+0x6d/0x8b
[103181.046216]  cdrv_open+0xe/0x30 [1_cdrv]
[103181.046218]  chrdev_open+0xc4/0x1b0
[103181.046219]  ? cdev_put.part.3+0x20/0x20
[103181.046220]  do_dentry_open+0x1df/0x3a0
[103181.046221]  vfs_open+0x2f/0x40
[103181.046222]  path_openat+0x2f9/0x16a0
[103181.046224]  ? unlock_page_memcg+0x12/0x20
[103181.046225]  ? page_add_file_rmap+0x13a/0x180
[103181.046226]  ? filemap_map_pages+0x181/0x3b0
[103181.046227]  do_filp_open+0x9b/0x110
[103181.046228]  ? __check_object_size+0xdb/0x1b0
[103181.046230]  ? __alloc_fd+0x46/0x170
[103181.046231]  do_sys_open+0x1ba/0x2e0
[103181.046231]  ? do_sys_open+0x1ba/0x2e0
[103181.046233]  ? __do_page_fault+0x2be/0x4d0
[103181.046234]  __x64_sys_openat+0x20/0x30
[103181.046235]  do_syscall_64+0x57/0x190
[103181.046236]  entry_SYSCALL_64_after_hwframe+0x5c/0xc1
[103181.046237] RIP: 0033:0x7f005cdb3c3e
[103181.046238] Code: 25 00 00 41 00 3d 00 00 41 00 74 48 48 8d 05 b1 0d 2e 00 8b 00 85 c0 75 69 89 f2 b8 01 01 00 00 48 89 fe bf 9c ff ff ff 0f 05 <48> 3d 00 f0 ff ff 0f 87 a6 00 00 00 48 8b 4c 24 28 64 48 33 0c 25
[103181.046239] RSP: 002b:00007ffc31b6a3a0 EFLAGS: 00000246 ORIG_RAX: 0000000000000101
[103181.046240] RAX: ffffffffffffffda RBX: 0000000000000000 RCX: 00007f005cdb3c3e
[103181.046240] RDX: 0000000000000002 RSI: 00005562945e79e8 RDI: 00000000ffffff9c
[103181.046240] RBP: 00007ffc31b6b430 R08: 00007f005d090d80 R09: 00007f005d090d80
[103181.046241] R10: 0000000000000000 R11: 0000000000000246 R12: 00005562945e7710
[103181.046241] R13: 00007ffc31b6b510 R14: 0000000000000000 R15: 0000000000000000
[103181.046242] cdrv_open() invoked
[103245.989382] CPU: 3 PID: 83632 Comm: a.out Tainted: G           OE     5.4.0-135-generic #152~18.04.2-Ubuntu
[103245.989383] Hardware name: VMware, Inc. VMware Virtual Platform/440BX Desktop Reference Platform, BIOS 6.00 05/19/2017
[103245.989383] Call Trace:
[103245.989388]  dump_stack+0x6d/0x8b
[103245.989391]  cdrv_write+0x12/0x30 [1_cdrv]
[103245.989393]  __vfs_write+0x1b/0x40
[103245.989394]  vfs_write+0xb1/0x1a0
[103245.989394]  ksys_write+0xa7/0xe0
[103245.989395]  __x64_sys_write+0x1a/0x20
[103245.989397]  do_syscall_64+0x57/0x190
[103245.989398]  entry_SYSCALL_64_after_hwframe+0x5c/0xc1
[103245.989399] RIP: 0033:0x7f005cdb4104
[103245.989400] Code: 89 02 48 c7 c0 ff ff ff ff c3 66 2e 0f 1f 84 00 00 00 00 00 66 90 48 8d 05 e1 08 2e 00 8b 00 85 c0 75 13 b8 01 00 00 00 0f 05 <48> 3d 00 f0 ff ff 77 54 f3 c3 66 90 41 54 55 49 89 d4 53 48 89 f5
[103245.989401] RSP: 002b:00007ffc31b6a408 EFLAGS: 00000246 ORIG_RAX: 0000000000000001
[103245.989402] RAX: ffffffffffffffda RBX: 0000000000000000 RCX: 00007f005cdb4104
[103245.989402] RDX: 000000000000000c RSI: 00007ffc31b6a420 RDI: 0000000000000003
[103245.989403] RBP: 00007ffc31b6b430 R08: 00007f005d0918c0 R09: 00007f005d29a4c0
[103245.989428] R10: 0000556296312010 R11: 0000000000000246 R12: 00005562945e7710
[103245.989429] R13: 00007ffc31b6b510 R14: 0000000000000000 R15: 0000000000000000
[103245.989430] cdrv_write() invoked:Rec'vd data of len = 12
[103276.257248] CPU: 2 PID: 83632 Comm: a.out Tainted: G           OE     5.4.0-135-generic #152~18.04.2-Ubuntu
[103276.257249] Hardware name: VMware, Inc. VMware Virtual Platform/440BX Desktop Reference Platform, BIOS 6.00 05/19/2017
[103276.257249] Call Trace:
[103276.257273]  dump_stack+0x6d/0x8b
[103276.257277]  cdrv_read+0x12/0x2e [1_cdrv]
[103276.257279]  __vfs_read+0x1b/0x40
[103276.257279]  vfs_read+0x8e/0x130
[103276.257280]  ksys_read+0xa7/0xe0
[103276.257280]  __x64_sys_read+0x1a/0x20
[103276.257282]  do_syscall_64+0x57/0x190
[103276.257284]  entry_SYSCALL_64_after_hwframe+0x5c/0xc1
[103276.257285] RIP: 0033:0x7f005cdb4031
[103276.257286] Code: fe ff ff 48 8d 3d cf 9b 0a 00 48 83 ec 08 e8 56 4b 02 00 66 0f 1f 44 00 00 48 8d 05 b1 09 2e 00 8b 00 85 c0 75 13 31 c0 0f 05 <48> 3d 00 f0 ff ff 77 57 f3 c3 0f 1f 44 00 00 41 54 55 49 89 d4 53
[103276.257287] RSP: 002b:00007ffc31b6a408 EFLAGS: 00000246 ORIG_RAX: 0000000000000000
[103276.257288] RAX: ffffffffffffffda RBX: 0000000000000000 RCX: 00007f005cdb4031
[103276.257288] RDX: 0000000000000bb8 RSI: 00007ffc31b6a420 RDI: 0000000000000003
[103276.257288] RBP: 00007ffc31b6b430 R08: 00007f005d0918c0 R09: 00007f005d29a4c0
[103276.257289] R10: 0000556296312010 R11: 0000000000000246 R12: 00005562945e7710
[103276.257289] R13: 00007ffc31b6b510 R14: 0000000000000000 R15: 0000000000000000
[103276.257290] cdrv_read() invoked.
                request to read 3000 bytes
[103304.082116] CPU: 3 PID: 83632 Comm: a.out Tainted: G           OE     5.4.0-135-generic #152~18.04.2-Ubuntu
[103304.082117] Hardware name: VMware, Inc. VMware Virtual Platform/440BX Desktop Reference Platform, BIOS 6.00 05/19/2017
[103304.082118] Call Trace:
[103304.082122]  dump_stack+0x6d/0x8b
[103304.082125]  cdrv_release+0xe/0x30 [1_cdrv]
[103304.082126]  __fput+0xc6/0x260
[103304.082127]  ____fput+0xe/0x10
[103304.082129]  task_work_run+0x9d/0xc0
[103304.082131]  exit_to_usermode_loop+0x109/0x130
[103304.082132]  do_syscall_64+0x170/0x190
[103304.082133]  entry_SYSCALL_64_after_hwframe+0x5c/0xc1
[103304.082134] RIP: 0033:0x7f005cdb4884
[103304.082135] Code: eb 89 e8 7f 42 02 00 66 2e 0f 1f 84 00 00 00 00 00 0f 1f 44 00 00 48 8d 05 61 01 2e 00 8b 00 85 c0 75 13 b8 03 00 00 00 0f 05 <48> 3d 00 f0 ff ff 77 3c f3 c3 66 90 53 89 fb 48 83 ec 10 e8 a4 fc
[103304.082136] RSP: 002b:00007ffc31b6a408 EFLAGS: 00000246 ORIG_RAX: 0000000000000003
[103304.082137] RAX: 0000000000000000 RBX: 0000000000000000 RCX: 00007f005cdb4884
[103304.082137] RDX: 0000556296312261 RSI: 0000556296312260 RDI: 0000000000000003
[103304.082137] RBP: 00007ffc31b6b430 R08: 00007f005d0918c0 R09: 00007f005d29a4c0
[103304.082138] R10: 0000556296312010 R11: 0000000000000246 R12: 00005562945e7710
[103304.082138] R13: 00007ffc31b6b510 R14: 0000000000000000 R15: 0000000000000000
[103304.082139] cdrv_release() invoked
 *
 */
