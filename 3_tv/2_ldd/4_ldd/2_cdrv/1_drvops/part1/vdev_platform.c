#include <linux/module.h>
#include <linux/platform_device.h>
#include "platform.h"

/* Callback to free the device after all references have gone away */
void vdev_release(struct device *dev)
{
	pr_info("Device released\n");
}

struct virtual_platform_data vdev_pdata[] = {
	[0] = {.size = 512,.permission = RDWR,.serial_number = "VDEV_1"},
};

/* Create two platform device */
struct platform_device platform_vdev_1 = {
	.name = "vDev-Ax",
	.id = 0,
	.dev = {
		.platform_data = &vdev_pdata[0],
		.release = vdev_release
	}
};

struct platform_device *platform_vdevs[] = {
	&platform_vdev_1,
};

static int __init vDev_platform_init(void)
{

	platform_add_devices(platform_vdevs, ARRAY_SIZE(platform_vdevs));
	pr_info("Platform device setup module loaded\n");
	return 0;
}

static void __exit vDev_platform_exit(void)
{
	/* Unregister platform-level device */
	platform_device_unregister(&platform_vdev_1);
	pr_info("Platform device setup module unloaded\n");
}

module_init(vDev_platform_init);
module_exit(vDev_platform_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("Register platform device");
