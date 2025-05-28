/*
 *
 *
 *
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/dmapool.h>

int direction = DMA_TO_DEVICE;

static void *kbuf;
static dma_addr_t handle;
static struct device *dev;
static int size = 512;

static void mydev_release(struct device *dev)
{
        pr_info("releasing device\n");
}



static int dma_init (void)
{
     static const u64 dmamask = DMA_BIT_MASK(32);
     void *pa;

     dev = kzalloc(sizeof(struct device), GFP_KERNEL);
     dev_set_name(dev, "dmastream");
     dev->dma_mask = (u64 *)&dmamask;
     dev->coherent_dma_mask = DMA_BIT_MASK(32);
     dev->release = mydev_release;
     device_register(dev);

   //  kbuf = kmalloc (size, GFP_KERNEL);
     kbuf = kmalloc (size, GFP_KERNEL | GFP_DMA);
    if (kbuf == NULL){
	    pr_err("allocation failed\n");
    	    return -ENOMEM;
    }

    /* populate data into buffer */

    handle = dma_map_single (dev, kbuf, size, direction);
     if (dma_mapping_error(dev, handle)) {
                pr_err("dma mapping failed\n");
                return -EINVAL;
        }

    pr_info("Kernel mapped @ %12p, DMA mapped @ %12p, size = %d\n", kbuf,
                (unsigned long *)handle, (int)size);

    pa = (void *)__pa(kbuf);
    pr_info("physcial address @ %12p\n",pa);

     strcpy((char *)kbuf, "mapped buffer");
     pr_info("DATA: %s\n", (char *)kbuf);

    return 0;
}
static void dma_exit (void)
{
	 dma_unmap_single (dev, handle, size, direction);
    	 kfree (kbuf);
	 device_unregister(dev);
}

module_init (dma_init);
module_exit (dma_exit);

MODULE_AUTHOR ("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION ("DMA interface test");
MODULE_LICENSE ("Dual MIT/GPL");


/* Rx
 * step 1: allocate buffer
 * step 2: dma_map_single(DMA_FROM_DEVICE) // mapping
 * step 3: Program DMA transfer
 * step 4: wait for DMA Transfer to complete
 * step 5: dma_unmap_single(DMA_FROM_DEVICE);//sync point (invalidation mem->cache)
 * step 6: Read data arrived into dma buffer.
 
 * Tx
 * step 1: allocate buffer
 * step 2: populate data into buffer
 * step 3: dma_map_single(DMA_TO_DEVICE) //flush from cache to memory
 * step 4: program DMA transfer
 * step 5: wait for transfer completion
 * step 6: dma_unmap_single(DMA_TO_DEVICE) //unmap dma_addr_t
 */
/*
 * insmod :
[29717.242516] Kernel mapped @     b3d447e2, DMA mapped @     e1595f53, size = 512
[29717.242518] physcial address @     6dd773a8
[29717.242520] DATA: mapped buffer

 Note for checking : DMA mapped address and Physical Address is not matching , that should match;
 Answerr : //  kbuf = kmalloc (size, GFP_KERNEL); // this without GFP_DMA flag // GFP KERNEL means normal ZOne


/* now below physical address and DMA address is matching
 * with GFP_DMA flag; kbuf = kmalloc (size, GFP_KERNEL | GFP_DMA);
 *
[30444.712340] Kernel mapped @     77228c5e, DMA mapped @     9a703206, size = 512
[30444.712343] physcial address @     9a703206
[30444.712345] DATA: mapped buffer


 */
