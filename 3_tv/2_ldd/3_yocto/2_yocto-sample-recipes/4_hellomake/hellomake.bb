SUMMARY = "Example recipe for first application"
DESCRIPTION = "This recipe serves as an example"
SECTION = "examples"
PR = "r1"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "file://hellomake.c \
	   file://Makefile    \
           "

S = "${WORKDIR}"


do_compile(){
	oe_runmake 
}

do_install(){
	oe_runmake install 'DESTDIR=${D}'
}
