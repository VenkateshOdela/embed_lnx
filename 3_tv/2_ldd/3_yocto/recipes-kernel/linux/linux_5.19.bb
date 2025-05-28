DESCRIPTION = "Linux kernel"
SECTION = "kernel"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM ?= "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"


LINUX_VERSION = "5.19.17"
LINUX_VERSION_EXTENSION = "-techveda"
DEPENDS += "bc-native bison-native util-linux-native xz-native lz4-native"

inherit kernel

PV = "${LINUX_VERSION}"

SRC_URI = "git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git;branch=linux-5.19.y"
SRCREV =  "2b525314c7b57eac29fe8b77a6589428e4a4f6dd"
S = "${WORKDIR}/git"

KERNEL_DEVICETREE_qemuarm = "versatile-pb.dtb"
KERNEL_DEVICETREE_beaglebone_yocto = "am355x-boneblack.dtb"

KERNEL_CONFIG_COMMAND_qemuarm = "oe_runmake -C ${S} CC="${KERNEL_CC}" LD="${KERNEL_LD}" O=${B} multi_v7_defconfig"
KERNEL_CONFIG_COMMAND_beaglebone-yocto = "oe_runmake -C ${S} CC="${KERNEL_CC}" LD="${KERNEL_LD}" O=${B} omap2plus_defconfig"


COMPATIBLE_MACHINE = "(qemuarm|beaglebone-yocto)"
