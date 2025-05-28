SUMMARY = "test app for vDev family of devices"
DESCRIPTION = "$SUMMARY"
SECTION = "testapps"
PR = "r1"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "file://vdevs_test.c"

S = "${WORKDIR}"


do_compile(){
	${CC} ${LDFLAGS} vdevs_test.c -o vdevtest
}

do_install(){
	install -d ${D}${bindir}
	install -m 0755 vdevtest ${D}${bindir}
}
