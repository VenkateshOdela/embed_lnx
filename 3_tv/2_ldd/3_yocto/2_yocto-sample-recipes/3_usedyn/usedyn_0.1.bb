SUMMARY = "Example recipe to add a test app"
DESCRIPTION = "First recipe"
SECTION = "examples"
LICENSE = "GPL-1.0-only"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/GPL-1.0-only;md5=e9e36a9de734199567a4d769498f743d"

SRC_URI = "file://testlib.c \
	  "
S = "${WORKDIR}"

DEPENDS += "libdyn"

do_compile() {
             ${CC} ${LDFLAGS} testlib.c -o testlib -ldyn     # Compile source file which resides at ${S}
}

do_install() {
             install -d ${D}${bindir}                   # Creates a ${bindir} at ${D}; where D is destination directory
             install -m 0755 testlib ${D}${bindir}     # Copies source(myhelloworld) file to ${D}${bindir} & assign mode
}
	
