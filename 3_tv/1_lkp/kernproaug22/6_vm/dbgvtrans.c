/*
 *
 *
 *
 *
 *
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/debugfs.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/uaccess.h>
#include <linux/sched/mm.h>
#include <linux/printk.h>
#include <linux/sched/signal.h>

#define OURMODNAME      "dbgvtrans"
#define FILENAME	"address"


static struct dentry *gparent;

static struct page *find_physical_page(struct vm_area_struct *vma,
                                       unsigned long addr)
{
        pud_t *pud;
        pmd_t *pmd;
        p4d_t *p4d;
        pgd_t *pgd;
        pte_t *pte;
        spinlock_t *ptl;
        struct page *page = NULL;
        struct mm_struct *mm = vma->vm_mm;

        pgd = pgd_offset(mm, addr);
        if (pgd_none(*pgd) || unlikely(pgd_bad(*pgd))) {
                goto out;
        }

        p4d = p4d_offset(pgd, addr);
        if (p4d_none(*p4d)) {
                printk("not mapped in p4d\n");
                goto out;
        }

        pud = pud_offset(p4d, addr);
        if (pud_none(*pud) || unlikely(pud_bad(*pud))) {
                goto out;
        }
        pmd = pmd_offset(pud, addr);
        if (pmd_none(*pmd) || unlikely(pmd_bad(*pmd))) {
                goto out;
        }
        pte = pte_offset_map_lock(mm, pmd, addr, &ptl);
        if (!pte)
                goto out;
        if (!pte_present(*pte))
                goto unlock;
        page = pfn_to_page(pte_pfn(*pte));
        if (!page)
		goto unlock;
        get_page(page);
 unlock:
        pte_unmap_unlock(pte, ptl);
 out:
        return page;
}

static void write_val(unsigned long addr, int val)
{
        struct vm_area_struct *vma;
        struct mm_struct *mm = current->mm; // Use current to memory descriptors of present process;
        struct page *page;
        unsigned long kernel_addr;

        down_read(&mm->mmap_lock);
        vma = find_vma(mm, addr);

        pr_info("write val invoked\n");

        if (vma && addr >= vma->vm_start && (addr + sizeof(val)) < vma->vm_end) {
                if (!(vma->vm_flags & VM_WRITE))
                        goto out;
                page = find_physical_page(vma, addr);
                if (!page)
                        goto out;
                kernel_addr = (unsigned long)page_address(page);
                kernel_addr += (addr & ~PAGE_MASK);
                *(int *)kernel_addr = val;
                put_page(page);
        }
 out:
        up_read(&mm->mmap_lock);
}

ssize_t dbg_write(struct file *file, const char __user *buffer, size_t count, loff_t *off)
{
	char buf[32];
	unsigned long vaddr;
	int data;

	simple_write_to_buffer(buf, 32, off, buffer, count);
	sscanf(buf, "%lx:%d", &vaddr, &data);
        pr_info("%s: vaddr %lx; Data %d\n", __func__,(unsigned long) vaddr, data);
        write_val(vaddr,data);
        return count;
}

static const struct file_operations dbg_fops = {
	.write = dbg_write,
};

static int debug_init(void)
{
	struct dentry *file1;

	if (!IS_ENABLED(CONFIG_DEBUG_FS)) {
		pr_warn("%s: debugfs unsupported! Aborting ...\n", OURMODNAME);
		return -EINVAL;
	}

	/* Create a dir under the debugfs mount point, whose name is the
	 * module name */
	gparent = debugfs_create_dir(OURMODNAME, NULL);
	if (!gparent) {
		pr_err("%s: debugfs_create_dir failed, aborting...\n",
			OURMODNAME);
		return -EINVAL;
	}

	file1 = debugfs_create_file(FILENAME, 0666, gparent, NULL, &dbg_fops);
	if (!file1) {
		pr_err("%s: debugfs_create_file failed, aborting...\n",
			OURMODNAME);
		goto out_fail;
	}
	pr_debug("%s <debugfs_mountpt>/%s/%s created\n",
		 OURMODNAME, OURMODNAME, FILENAME);

	return 0;
out_fail:
	debugfs_remove_recursive(gparent);
	return -EINVAL;
}

static void debug_cleanup(void)
{
	debugfs_remove_recursive(gparent);
	pr_info("%s removed\n", OURMODNAME);
}

module_init(debug_init);
module_exit(debug_cleanup);

MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("Debugfs");
MODULE_LICENSE("Dual MIT/GPL");

