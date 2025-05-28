/*
 *
 *
 *
 */

#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/dmapool.h>


static void *kbuf;
static dma_addr_t handle;
static size_t size = 4096;
static struct device *dev;


static void mydev_release(struct device *dev)
{
        pr_info("releasing device\n");
}


static int dma_init(void)
{
	static const u64 dmamask = DMA_BIT_MASK(32); // setup 32 bit DMA
	void *pa;

	dev = kzalloc(sizeof(struct device), GFP_KERNEL);
	dev_set_name(dev, "dmatest");
	dev->dma_mask = (u64 *)&dmamask;
	dev->coherent_dma_mask = DMA_BIT_MASK(32);
	dev->release = mydev_release;
	device_register(dev);  // Registering virtual device with kernel

	/* for above virtual device setting up DMA below */

	/* dma_alloc_coherent method */
	/* dma address is returned in handle */
	/* linear address is returned in kbuf */
	kbuf = dma_alloc_coherent(dev, size, &handle, GFP_KERNEL | GFP_DMA32);
	if ((kbuf == NULL) || (handle == 0)){
		pr_err("coherent allocation failed\n");
		return -ENOMEM;
	}

	/* printing both addresses */
	/* in x86 machine, CPU physical address is directly used as DMA address,DMAs does not have unique bus address in x86
	 * and there is no IOMEM*/
	pr_info("kbuf=%12p, handle=%12p, size = %d\n", kbuf,
		(unsigned long *)handle, (int)size);

	/* passing logical address and getting physical address by looikng into page table*/
	/* this physical address pa should match with handle */
	pa = (void *)__pa(kbuf);
	pr_info("physcial address @ %12p\n",pa);

	strcpy((char *)kbuf, "DMA coherent mapped buffer");
	pr_info("DATA: %s\n", (char *)kbuf);

	return 0;
}

static void dma_exit(void)
{
	dma_free_coherent(dev, size, kbuf, handle);
        device_unregister(dev);
}

module_init(dma_init);
module_exit(dma_exit);

MODULE_AUTHOR("Team Veda");
MODULE_DESCRIPTION("DMA interface test");
MODULE_LICENSE("GPL v2");

/*
 * insmod
[29501.302008] dmatest: loading out-of-tree module taints kernel.
[29501.316472] dmatest: module verification failed: signature and/or required key missing - tainting kernel
[29501.339143] kbuf=    b6501731, handle=    9f71b276, size = 4096
[29501.339144] physcial address @     9f71b276
[29501.339145] DATA: DMA coherent mapped buffer
 *
 *
 * rmmod
 *[29558.757852] releasing device
 */
