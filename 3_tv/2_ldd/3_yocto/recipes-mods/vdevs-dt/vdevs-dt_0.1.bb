SUMMARY = "vritual platform device(DT enumerated) driver"
DESCRIPTION = "${SUMMARY}"
LICENSE = "GPL-1.0-only"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/GPL-1.0-only;md5=e9e36a9de734199567a4d769498f743d"

inherit module

SRC_URI = "file://Makefile \
           file://vdevs_dt_drv.c \
	   file://vdevs_platdata.h \
          "

S = "${WORKDIR}"

# The inherit of module.bbclass will automatically name module packages with
# "kernel-module-" prefix as required by the oe-core build environment.

RPROVIDES:${PN} += "kernel-module-vdevsdt"
