#include <linux/module.h>
#include <linux/mtd/mtd.h>

static int my_flash_probe(struct platform_device *pdev)
{
    struct mtd_info *mtd;
    int ret;

    /* Allocate a new mtd_info structure */
    mtd = kzalloc(sizeof(*mtd), GFP_KERNEL);
    if (!mtd)
        return -ENOMEM;

    /* Initialize the mtd_info structure */
    mtd->name = "my_flash";
    mtd->size = SIZE;
    mtd->erase = my_flash_erase;
    mtd->read = my_flash_read;
    mtd->write = my_flash_write;
    mtd->priv = priv;

    /* Register the MTD device with the kernel */
    ret = mtd_device_register(mtd, NULL, 0);
    if (ret) {
        kfree(mtd);
        return ret;
    }

    /* Save the mtd_info structure as platform data */
    platform_set_drvdata(pdev, mtd);

    return 0;
}

static int my_flash_remove(struct platform_device *pdev)
{
    struct mtd_info *mtd = platform_get_drvdata(pdev);

    /* Unregister the MTD device from the kernel */
    mtd_device_unregister(mtd);

    /* Free the mtd_info structure */
    kfree(mtd);

    return 0;
}

static struct platform_driver my_flash_driver = {
    .probe = my_flash_probe,
    .remove = my_flash_remove,
    .driver = {
        .name = "my_flash",
    },
};

module_platform_driver(my_flash_driver);

