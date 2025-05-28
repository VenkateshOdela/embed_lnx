#
#dynamic library recipe
#
DESCRIPTION = "Simple Dynamic library"
SECTION = "custlib"
LICENSE = "CLOSED"
PR = "r0"

SRC_URI = "file://sample.c \
	   file://dyn.h \
          "
S = "${WORKDIR}"

# The PV variable contains the package version. If it is not defined in the recipe, it defaults to 1.0.
# Yocto requires that the ELF tag SONAME must be included in the library when it is built, thus, the flag “-Wl,-soname,libdyn.so.${PV}” has been added to the 
# do_compile step.


do_compile() {
			${CC} ${CFLAGS} -c -fPIC sample.c -o sample.o 
			${CC} ${LDFLAGS} -shared -Wl,-soname,libdyn.so.1 -o libdyn.so.${PV}.0 sample.o
}

do_install() {
				install -d ${D}${libdir}
				oe_soinstall ${S}/libdyn.so.${PV}.0 ${D}${libdir}
				install -d ${D}${includedir}
                                install -m 0755 ${S}/*.h ${D}${includedir}
}
