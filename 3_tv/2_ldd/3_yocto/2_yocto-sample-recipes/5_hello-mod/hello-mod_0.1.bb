SUMMARY = "Example of how to build an external Linux kernel module"
DESCRIPTION = "${SUMMARY}"
#LICENSE = "GPL-1.0-only"
#LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/GPL-1.0-only;md5=e9e36a9de734199567a4d769498f743d"

LICENSE = "GPL-1.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/GPL-1.0;md5=e9e36a9de734199567a4d769498f743d"

#LICENSE = "MIT"
#LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

inherit module

SRC_URI = "file://Makefile \
           file://hello.c \
           file://COPYING \
          "

S = "${WORKDIR}"

# The inherit of module.bbclass will automatically name module packages with
# "kernel-module-" prefix as required by the oe-core build environment.

RPROVIDES:${PN} += "kernel-module-hello"
