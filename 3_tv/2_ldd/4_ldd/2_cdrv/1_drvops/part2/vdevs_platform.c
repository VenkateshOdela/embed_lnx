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
	[1] = {.size = 512,.permission = RDWR,.serial_number = "VDEV_2"},
	[2] = {.size = 1024,.permission = RDONLY,.serial_number = "VDEV_3"},
	[3] = {.size = 2048,.permission = WRONLY,.serial_number = "VDEV_4"}
};

/* Create two platform device */
struct platform_device platform_vdev_1 = {
	.name = "vDev-Ax",
	.id = 0,
	.dev = {
		.platform_data = &vdev_pdata[0],
		.release = vdev_release}
};

struct platform_device platform_vdev_2 = {
	.name = "vDev-Bx",
	.id = 1,
	.dev = {
		.platform_data = &vdev_pdata[1],
		.release = vdev_release}
};

struct platform_device platform_vdev_3 = {
	.name = "vDev-Cx",
	.id = 2,
	.dev = {
		.platform_data = &vdev_pdata[2],
		.release = vdev_release}
};

struct platform_device platform_vdev_4 = {
	.name = "vDev-Dx",
	.id = 3,
	.dev = {
		.platform_data = &vdev_pdata[3],
		.release = vdev_release}
};

struct platform_device *platform_vdevs[] = {
	&platform_vdev_1,
	&platform_vdev_2,
	&platform_vdev_3,
	&platform_vdev_4
};

static int __init vDev_platform_init(void)
{
	/* Register platform-level device */
	platform_add_devices(platform_vdevs, ARRAY_SIZE(platform_vdevs));

	pr_info("Platform device setup module loaded\n");

	return 0;
}

static void __exit vDev_platform_exit(void)
{
	/* Unregister platform-level device */
	platform_device_unregister(&platform_vdev_1);
	platform_device_unregister(&platform_vdev_2);
	platform_device_unregister(&platform_vdev_3);
	platform_device_unregister(&platform_vdev_4);

	pr_info("Platform device setup module unloaded\n");
}

module_init(vDev_platform_init);
module_exit(vDev_platform_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Raghu Bharadwaj <raghu@techveda.org>");
MODULE_DESCRIPTION("Register platform devices");
