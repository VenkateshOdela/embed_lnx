/* Sample Linux PCI device driver */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/pci.h>

#define MY_DRIVER "pcie_skel"


struct my_driver_priv {
    u8 __iomem *hwmem;
};


void release_device(struct pci_dev *pdev)
{
    /* Disable IRQ #42*/
    free_irq(42, pdev);
    /* Free memory region */
    pci_release_region(pdev, pci_select_bars(pdev, IORESOURCE_MEM));
    /* And disable device */
    pci_disable_device(pdev);
}

/* */

static irqreturn_t irq_handler(int irq, void *cookie)
{
   (void) cookie;
   printk("Handle IRQ #%d\n", irq);
   return IRQ_HANDLED;
}

/* Reqest interrupt and setup handler */
int set_interrupts(struct pci_dev *pdev)
{
    /* We want MSI interrupt, 3 lines (just an example) */
    int ret = pci_alloc_irq_vectors(pdev, 3, 3, PCI_IRQ_MSI);

    if (ret < 0) {
        return ret;
    }

    /* Request IRQ #42 */
    return request_threaded_irq(42, irq_handler, NULL, 0, "TEST IRQ", pdev);
}

/* Write some data to the device */
void write_sample_data(struct pci_dev *pdev)
{
    int data_to_write = 0xDEAD; 

    struct my_driver_priv *drv_priv = (struct my_driver_priv *) pci_get_drvdata(pdev);

    if (!drv_priv) {
        return;
    }

    /* Write 32-bit data to the device memory */
    iowrite32(data_to_write, drv_priv->hwmem);
}

/* This function is called by the kernel */
static int skel_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
    int bar, err;
    u16 vendor, device;
    unsigned long mmio_start,mmio_len;
    struct my_driver_priv *drv_priv;

      /* Enable device memory */
    err = pci_enable_device_mem(pdev);
    if (err) {
        return err;
    }

    /* Let's read data from the PCI device configuration registers */
    pci_read_config_word(pdev, PCI_VENDOR_ID, &vendor);
    pci_read_config_word(pdev, PCI_DEVICE_ID, &device);

    printk(KERN_INFO "Device vid: 0x%X pid: 0x%X\n", vendor, device);

    /* Request IO BAR */
    bar = pci_select_bars(pdev, IORESOURCE_MEM);


    /* Request memory region for the BAR */
    err = pci_request_region(pdev, bar, MY_DRIVER);

    if (err) {
        pci_disable_device(pdev);
        return err;
    }

    /* Get start and stop memory offsets */
    mmio_start = pci_resource_start(pdev, 0);
    mmio_len = pci_resource_len(pdev, 0);

    /* Allocate memory for the driver private data */
    drv_priv = kzalloc(sizeof(struct my_driver_priv), GFP_KERNEL);

    if (!drv_priv) {
        release_device(pdev);
        return -ENOMEM;
    }

    /* Remap BAR to the local pointer */
    drv_priv->hwmem = ioremap(mmio_start, mmio_len);

    if (!drv_priv->hwmem) {
       release_device(pdev);
       return -EIO;
    }

    /* Set driver private data */
    /* Now we can access mapped "hwmem" from the any driver's function */
    pci_set_drvdata(pdev, drv_priv);

    write_sample_data(pdev);

    return set_interrupts(pdev);
}

/* Clean up */
static void skel_remove(struct pci_dev *pdev)
{
    struct my_driver_priv *drv_priv = pci_get_drvdata(pdev);

    if (drv_priv) {
        if (drv_priv->hwmem) {
            iounmap(drv_priv->hwmem);
        }

        pci_free_irq_vectors(pdev);

        kfree(drv_priv);
    }

    release_device(pdev);
}

static struct pci_device_id my_driver_id_table[] = {
    { PCI_DEVICE(0x010F, 0x0F0E) },
    {0,}
};

MODULE_DEVICE_TABLE(pci, my_driver_id_table);

/* Driver registration structure */
static struct pci_driver skel_drv = {
    .name = MY_DRIVER,
    .id_table = my_driver_id_table,
    .probe = skel_probe,
    .remove = skel_remove
};

module_pci_driver(skel_drv);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Team veda <support@techveda.com>");
MODULE_DESCRIPTION("PCIe Skel drv");
