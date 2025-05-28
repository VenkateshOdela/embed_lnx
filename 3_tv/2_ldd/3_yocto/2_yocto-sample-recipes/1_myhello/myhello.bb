SUMMARY = "Example recipe for first application"
DESCRIPTION = "This recipe serves as an example"
SECTION = "examples"
PR = "r1"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "file://myhello.c"

S = "${WORKDIR}"


do_compile(){
	${CC} ${LDFLAGS} myhello.c -o myhello
}

do_install(){
	install -d ${D}${bindir}
	install -m 0755 myhello ${D}${bindir}
}
