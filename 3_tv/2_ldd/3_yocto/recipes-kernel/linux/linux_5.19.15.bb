DESCRIPTION = "Linux kernel"
SECTION = "kernel"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM ?= "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"

DEPENDS += "bc-native bison-native openssl-native util-linux-native xz-native lz4-native"
inherit kernel

LINUX_VERSION = "5.19.15"
LINUX_VERSION_EXTENSION = "-techveda"

PV = "${LINUX_VERSION}"

SRC_URI = "https://mirrors.edge.kernel.org/pub/linux/kernel/v5.x/linux-5.19.15.tar.xz \
           file://0001-devtree-and-other-custs.patch \
           "
SRC_URI[sha256sum] = "4686e33847db3ec58e1043bc0c806472aae694b2c413bb7d8da91a5c9a55eb1b"

S = "${WORKDIR}/linux-${PV}"

KERNEL_DEVICETREE_qemuarm = "versatile-pb.dtb"
KERNEL_DEVICETREE_beaglebone_yocto = "am355x-boneblack.dtb"

KERNEL_CONFIG_COMMAND_qemuarm = "oe_runmake -C ${S} CC="${KERNEL_CC}" LD="${KERNEL_LD}" O=${B} multi_v7_defconfig"
KERNEL_CONFIG_COMMAND_beaglebone-yocto = "oe_runmake -C ${S} CC="${KERNEL_CC}" LD="${KERNEL_LD}" O=${B} omap2plus_defconfig"

COMPATIBLE_MACHINE = "(qemuarm|beaglebone-yocto)"
