DESCRIPTION = "Linux kernel"
SECTION = "kernel"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM ?= "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"

DEPENDS += "bc-native bison-native openssl-native util-linux-native xz-native lz4-native"
inherit kernel

LINUX_VERSION = "${LINUX_VMAJOR}.${LINUX_VMINOR}.${LINUX_VPATCH}"
LINUX_BRANCH = "linux-${LINUX_VMAJOR}.${LINUX_VMINOR}.y"
LINUX_VERSION_EXTENSION = "-techveda"

PV = "${LINUX_VERSION}"

LINUX_GIT_URI = "git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git;branch=${LINUX_BRANCH};protocol=https"
LINUX_TARBALL_URI = "https://cdn.kernel.org/pub/linux/kernel/v${LINUX_VMAJOR}.x/linux-${LINUX_VERSION}.tar.xz"

# The download method may either be 'git' or 'tarball'.
LINUX_STABLE_DOWNLOAD ??= "tarball"
require linux-stable-${LINUX_STABLE_DOWNLOAD}.inc
