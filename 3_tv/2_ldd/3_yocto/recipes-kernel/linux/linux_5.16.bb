DESCRIPTION = "Linux kernel"
SECTION = "kernel"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM ?= "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"


LINUX_VERSION = "5.16.20"
LINUX_VERSION_EXTENSION = "-techveda"
DEPENDS += "bc-native bison-native util-linux-native xz-native lz4-native"

inherit kernel

PV = "${LINUX_VERSION}"

SRC_URI = "git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git;branch=linux-5.16.y"
#SRCREV = "5fd3e07fd10e79694bff69fff1d38e97b47e77f0"
SRCREV = "c19a885e12f114b799b5d0d877219f0695e0d4de"
S = "${WORKDIR}/git"

KERNEL_DEVICETREE_qemuarm = "versatile-pb.dtb"
KERNEL_DEVICETREE_beaglebone_yocto = "am355x-boneblack.dtb"

KERNEL_CONFIG_COMMAND_qemuarm = "oe_runmake -C ${S} CC="${KERNEL_CC}" LD="${KERNEL_LD}" O=${B} multi_v7_defconfig"
KERNEL_CONFIG_COMMAND_beaglebone-yocto = "oe_runmake -C ${S} CC="${KERNEL_CC}" LD="${KERNEL_LD}" O=${B} omap2plus_defconfig"


COMPATIBLE_MACHINE = "(qemuarm|beaglebone-yocto)"
