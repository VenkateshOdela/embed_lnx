/*
 *
 */

#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/dmapool.h>

#define BUFSIZE 128

static struct device *dev;
u32 *wbuf, *wbuf2, *wbuf3;
struct scatterlist sg[3];

static void mydev_release(struct device *dev)
{
	pr_info("%s:Releasing device\n", __func__);
}

static int dma_init(void)
{
	int ret;
	int i = 0;
	static const u64 dmamask = DMA_BIT_MASK(32);
	unsigned long hw_address[3];
	struct scatterlist *sglist;
	

	dev = kzalloc(sizeof(struct device), GFP_KERNEL);
	dev_set_name(dev, "dmasgtestm");
	dev->dma_mask = (u64 *) & dmamask;
	dev->coherent_dma_mask = DMA_BIT_MASK(32);
	dev->release = mydev_release;
	device_register(dev);

	wbuf = kzalloc(BUFSIZE, GFP_DMA);
	if (!wbuf) {
		pr_err("error wbuf !!!!!!!!!!!\n");
		return -1;
	}

	wbuf2 = kzalloc(BUFSIZE, GFP_DMA);
	if (!wbuf2) {
		pr_err("error wbuf2 !!!!!!!!!!!\n");
		return -1;
	}

	wbuf3 = kzalloc(BUFSIZE, GFP_DMA);
	if (!wbuf3) {
		pr_err("error wbuf3 !!!!!!!!!!!\n");
		return -1;
	}

	pr_info("%s: wbuf = %12p wbuf2 = %12p wbuf3 = %12p\n", __func__,wbuf, wbuf2, wbuf3);

	sg_init_table(sg, 3);
	sg_set_buf(&sg[0], wbuf, BUFSIZE);
	sg_set_buf(&sg[1], wbuf2, BUFSIZE);
	sg_set_buf(&sg[2], wbuf3, BUFSIZE);
	ret = dma_map_sg(dev, sg, 3, DMA_TO_DEVICE); // to get three buffers together physically contiguous

	// dma_map_seg is used when we need large MBs of memory;

	//this is when DMA need morethan 8MB,typically required graphic cards

	// for scatter gather - IOMMU must be there;
	// scatter gather only possible in arm platforms; (not possible in x86)

	// with arch with out IOMMU,HOW to assign large MBs of memory,mem paramter in booting time : Reserving Memory at boot time;
	// system RAM, Showing 7.5GB, means it is reserved during boot time;
	for_each_sg(sg, sglist, 3, i) {
                hw_address[i] = sg_dma_address(sglist);
		pr_info("%s: DMA Mapped @ %12p\n", __func__,(void *)hw_address[i]);
	}

	return 0;
}

static void dma_exit(void)
{
	dma_unmap_sg(dev, sg, 3, DMA_TO_DEVICE);
	kfree(wbuf);
	kfree(wbuf2);
	kfree(wbuf3);
	device_unregister(dev);
}

module_init(dma_init);
module_exit(dma_exit);

MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("DMA scatter/gather test");
MODULE_LICENSE("GPL v2");

/*
 * Three 128bytes of buffer are separatly allocated contiguous
 * but, three together are not physically contiguous.
 *
 * These program makes 3 buffers physically contiguous;
 */

/*
 * 	// dma_map_seg is used when we need large MBs of memory;

	//this is when DMA need morethan 8MB,typically required graphic cards

	// for scatter gather - IOMMU must be there;
	// scatter gather only possible in arm platforms; (not possible in x86)

	// with arch with out IOMMU,HOW to assign large MBs of memory,mem paramter in booting time : Reserving Memory at boot time;
	// system RAM, Showing 7.5GB, means it is reserved during boot time;
 *
 *
 *
 * Boot Time Memory Setup :
 *
 * MEM=512
 *
 * ioremap()-> drivers use ioremap to map physical memory address into virtual address space;
	 *
	 * kdump() , kexec()
	 *
	 * drawback : like we are assigning early at boot time, as at during run time at may be required only at few intervals;
	 *
	 * ioremap(start_address, size);
	 *
	 * cat /proc/meminfo
	 *
	 * user@ubuntu:~/git_embd_linux/embd_linux/ds/linked_lists/single_linkedlist$ cat /proc/meminfo
		MemTotal:        3994696 kB
		MemFree:          119536 kB
		MemAvailable:    1168388 kB
	 *
	 * The linux ioremap() function causes the memory to be setup as device memory in the MMU which
	 * should be slower than normal memory;
	 *
	 * with MEM we are reserving memory, with which memory is not visible to system for other operation anymore;
	 *
	 * drawback :  system performance is reducing if reserving this memory;
 *
 * Solution : CMA : Contiguous Memory Allocator;
 *  - CMA=
 *  - CMA is recommended for DMA . (MEM= is not recommended for DMA)
 *  - is only accessible in DMA framework via dma_alloc_coherent()
 *  - there is no ioremap here;
 *  -Contigouos memory allocator varaible is present in make menuconfig;
 *
 *
 *  GCMA:
 *
 *
 *
 *
 */
