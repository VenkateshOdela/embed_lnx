/*
  * constructing page table with five level page table model
  *
  *  checking that is assingned kmalloc address is mapped to physical RAM
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/highmem.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>

unsigned long vaddr = 0;

static unsigned long vaddr2paddr(unsigned long vaddr)
{
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    char *kp;

    unsigned long offset = vaddr;
    struct page *page = NULL;

    pr_info("Accessing translation tables of the process %s with id %d", current->comm, current->pid);

    pgd = pgd_offset(current->mm, vaddr);  // pgd_offset
    pr_info("pgd_val = 0x%lx\n", pgd_val(*pgd));
    pr_info("pgd_index = %lu\n", pgd_index(vaddr));
    if (pgd_none(*pgd)) {
        pr_err("not mapped in pgd\n");
        return -1;
    }

    p4d = p4d_offset(pgd, vaddr);  // p4d_offset
    pr_info("p4d_val = 0x%lx\n", p4d_val(*p4d));
    if (p4d_none(*p4d)) {
        pr_err("not mapped in p4d\n");
        return -1;
    }

    pud = pud_offset(p4d, vaddr); // pud_offset
    pr_info("pud_val = 0x%lx\n", pud_val(*pud));
    if (pud_none(*pud)) {
        pr_err("not mapped in pud\n");
        return -1;
    }

    pmd = pmd_offset(pud, vaddr); // pmd_offset
    pr_info("pmd_val = 0x%lx\n", pmd_val(*pmd));
    pr_info("pmd_index = %lu\n", pmd_index(vaddr));
    if (pmd_none(*pmd)) {
        pr_err("not mapped in pmd\n");
        return -1;
    }

    pte = pte_offset_map(pmd, vaddr); // pte_offset_map
    pr_info("pte_val = 0x%lx\n", pte_val(*pte));
    pr_info("pte_index = %lu\n", pte_index(vaddr));
    if (pte_none(*pte)) {
        pr_err("not mapped in pte\n");
        return -1;
    }

    page = pte_page(*pte); // pte_page
    if(page)
        pr_err("Page frame struct is @ %p\n", page);


     kp = page_address(page); // it will give start address
     offset = offset & 0x0fff; // 12 bit offset
     kp[offset]='T';  // writing  One BYte;
     pr_info("%s:%s\n",__func__,kp+offset); // not reaching this page got error

     pte_unmap(pte);
     return 0;
}

static int __init v2p_init(void)
{

    unsigned long pgaddr = 0;

    pr_info("\n");

    vaddr = (unsigned long)kmalloc(1000 * sizeof(char),GFP_KERNEL); //kmalloc
    if (vaddr == 0) {
        pr_err("kmalloc failed..\n");
        return 0;
    }
    strcpy((char *)vaddr, "techveda:kmalloc allocation"); //string is written to address allocate from kmalloc
    pr_info("kmalloc_vaddr=0x%lx\n", vaddr);
    vaddr2paddr(vaddr);
  
    kfree((void *)vaddr);
    pr_info("\n\n");

    vaddr = 0;
    vaddr = (unsigned long)vmalloc(1000 * sizeof(char)); // vmalloc
    if (vaddr == 0) {
        pr_err("vmalloc failed..\n");
        return 0;
    }
    strcpy((char *)vaddr, "techveda:vmalloc allocation");
    pr_info("vmalloc_vaddr=0x%lx\n", vaddr);
    vaddr2paddr(vaddr);
 
    pr_info("\n\n");

    pgaddr = __get_free_page(GFP_KERNEL); // page alloctor
    if (pgaddr == 0) {
        pr_err("__get_free_page failed..\n");
        return 0;
    }
    strcpy((char *)pgaddr, "techveda:page allocation");
    vaddr2paddr(pgaddr);

    vfree((void *)vaddr);
    free_page(pgaddr);
    
    return 0;
}

static void __exit v2p_exit(void)
{
    pr_info("vaddr to paddr module is leaving..\n");
}
module_init(v2p_init);
module_exit(v2p_exit);

MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("address trans");
MODULE_LICENSE("Dual MIT/GPL");


/*
 * Five Level Page Table:
 *
 * pgd_t 9 bits;
 * p4d_t 9 bits;
 * pud_t 9 bits;
 * pmd_t 9 bits;
 * pte_t 9 bits;
 * Physical Ram(4K) 12 bits;
 *
 * 57 bits
 */

/*
 * user@ubuntu:~/git_embd_linux/embd_linux/tv/lkp/kernproaug22/vm$ dmesg

[45330.604962] kmalloc_vaddr=0xffff9a2fd26f2800
[45330.604964] Accessing translation tables of the process insmod with id 10709
[45330.604966] pgd_val = 0x107202067
[45330.604967] pgd_index = 308
[45330.604967] p4d_val = 0x107202067
[45330.604968] pud_val = 0x8f676063
[45330.604969] pmd_val = 0x80000000526000e3
[45330.604969] pmd_index = 147
[45330.604970] pte_val = 0xbb1cd38061a2258b
[45330.604970] pte_index = 242
[45330.604972] Page frame struct is @ 000000000672a18a
[45330.608106] general protection fault: 0000 [#1] SMP PTI
[45330.608117] CPU: 2 PID: 10709 Comm: insmod Tainted: G           OE     5.4.0-126-generic #142~18.04.1-Ubuntu
[45330.608119] Hardware name: VMware, Inc. VMware Virtual Platform/440BX Desktop Reference Platform, BIOS 6.00 05/19/2017
[45330.608960] RIP: 0010:vaddr2paddr+0x2b6/0x2c7 [ptdump]
[45330.608967] Code: 35 f4 c3 41 81 e4 ff 0f 00 00 48 c7 c6 10 f3 4f c0 48 c7 c7 97 f1 4f c0 48 c1 fa 06 48 c1 e2 0c 48 03 15 15 35 f4 c3 4c 01 e2 <c6> 02 54 e8 19 94 5c c3 31 c0 5b 41 5c 41 5d 5d c3 55 48 c7 c7 68
[45330.608969] RSP: 0018:ffffa7f6c64fbc30 EFLAGS: 00010206
[45330.608971] RAX: 0000000000000027 RBX: 00001303c1868880 RCX: 0000000000000006
[45330.608973] RDX: 000c6dafe1a22800 RSI: ffffffffc04ff310 RDI: ffffffffc04ff197
[45330.608974] RBP: ffffa7f6c64fbc48 R08: 00000000000006e6 R09: 637572747320656d
[45330.608975] R10: ffff9a2fd26f2800 R11: 3030303030303030 R12: 0000000000000800
[45330.608976] R13: 00000000000000f2 R14: ffffa7f6c64fbe68 R15: ffffffffc0500000
[45330.608978] FS:  00007fac30de4540(0000) GS:ffff9a30b9c80000(0000) knlGS:0000000000000000
[45330.608980] CS:  0010 DS: 0000 ES: 0000 CR0: 0000000080050033
[45330.608981] CR2: 000055af603c11e8 CR3: 00000000b48e2002 CR4: 00000000003606e0
[45330.609039] Call Trace:
[45330.609870]  ? 0xffffffffc0503000
[45330.609877]  v2p_init+0x75/0x1000 [ptdump]
[45330.612791]  do_one_initcall+0x4a/0x200
[45330.613326]  ? kfree+0x22b/0x240
[45330.614997]  ? _cond_resched+0x19/0x40
[45330.615039]  ? kmem_cache_alloc_trace+0x213/0x230
[45330.615452]  do_init_module+0x4f/0x20f
[45330.616213]  load_module+0x1c9b/0x22d0
[45330.617109]  __do_sys_finit_module+0xfc/0x120
[45330.617120]  ? __do_sys_finit_module+0xfc/0x120
[45330.617125]  __x64_sys_finit_module+0x1a/0x20
[45330.617130]  do_syscall_64+0x57/0x190
[45330.617134]  entry_SYSCALL_64_after_hwframe+0x44/0xa9
[45330.617137] RIP: 0033:0x7fac30907539
[45330.617142] Code: 00 f3 c3 66 2e 0f 1f 84 00 00 00 00 00 0f 1f 40 00 48 89 f8 48 89 f7 48 89 d6 48 89 ca 4d 89 c2 4d 89 c8 4c 8b 4c 24 08 0f 05 <48> 3d 01 f0 ff ff 73 01 c3 48 8b 0d 1f f9 2c 00 f7 d8 64 89 01 48
[45330.617144] RSP: 002b:00007fff1a660118 EFLAGS: 00000246 ORIG_RAX: 0000000000000139
[45330.617147] RAX: ffffffffffffffda RBX: 000055da8e2367b0 RCX: 00007fac30907539
[45330.617149] RDX: 0000000000000000 RSI: 000055da8dca9cee RDI: 0000000000000003
[45330.617150] RBP: 000055da8dca9cee R08: 0000000000000000 R09: 00007fac30bda000
[45330.617152] R10: 0000000000000003 R11: 0000000000000246 R12: 0000000000000000
[45330.617153] R13: 000055da8e236760 R14: 0000000000000000 R15: 0000000000000000
[45330.617156] Modules linked in: ptdump(OE+) rfcomm bnep intel_rapl_msr intel_rapl_common crct10dif_pclmul crc32_pclmul ghash_clmulni_intel snd_ens1371 snd_ac97_codec gameport aesni_intel ac97_bus crypto_simd cryptd glue_helper snd_pcm rapl vmw_balloon snd_seq_midi snd_seq_midi_event snd_rawmidi joydev input_leds serio_raw btusb snd_seq btrtl btbcm btintel bluetooth snd_seq_device snd_timer ecdh_generic ecc snd soundcore vmw_vsock_vmci_transport vsock vmw_vmci mac_hid sch_fq_codel vmwgfx ttm drm_kms_helper drm fb_sys_fops syscopyarea sysfillrect sysimgblt parport_pc ppdev lp parport ip_tables x_tables autofs4 hid_generic psmouse mptspi e1000 mptscsih ahci mptbase libahci scsi_transport_spi i2c_piix4 pata_acpi usbhid hid
[45330.617224] ---[ end trace 7f0cfbf32aa1eb54 ]---
[45330.617230] RIP: 0010:vaddr2paddr+0x2b6/0x2c7 [ptdump]
[45330.617233] Code: 35 f4 c3 41 81 e4 ff 0f 00 00 48 c7 c6 10 f3 4f c0 48 c7 c7 97 f1 4f c0 48 c1 fa 06 48 c1 e2 0c 48 03 15 15 35 f4 c3 4c 01 e2 <c6> 02 54 e8 19 94 5c c3 31 c0 5b 41 5c 41 5d 5d c3 55 48 c7 c7 68
[45330.617235] RSP: 0018:ffffa7f6c64fbc30 EFLAGS: 00010206
[45330.617237] RAX: 0000000000000027 RBX: 00001303c1868880 RCX: 0000000000000006
[45330.617239] RDX: 000c6dafe1a22800 RSI: ffffffffc04ff310 RDI: ffffffffc04ff197
[45330.617241] RBP: ffffa7f6c64fbc48 R08: 00000000000006e6 R09: 637572747320656d
[45330.617242] R10: ffff9a2fd26f2800 R11: 3030303030303030 R12: 0000000000000800
[45330.617244] R13: 00000000000000f2 R14: ffffa7f6c64fbe68 R15: ffffffffc0500000
[45330.617247] FS:  00007fac30de4540(0000) GS:ffff9a30b9c80000(0000) knlGS:0000000000000000
[45330.617248] CS:  0010 DS: 0000 ES: 0000 CR0: 0000000080050033
[45330.617250] CR2: 000055af603c11e8 CR3: 00000000b48e2002 CR4: 00000000003606e0
 *
 */
