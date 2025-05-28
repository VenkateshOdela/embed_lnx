# Recipe created by recipetool
# This is the basis of a recipe and may need further editing in order to be fully functional.
# (Feel free to remove these comments when editing.)

# WARNING: the following LICENSE and LIC_FILES_CHKSUM values are best guesses - it is
# your responsibility to verify that the values are complete and correct.
LICENSE = "GPL-3.0-only"
LIC_FILES_CHKSUM = "file://COPYING;md5=d32239bcb673463ab874e80d47fae504"

SRC_URI = "https://alpha.gnu.org/gnu/hello/hello-${PV}.tar.gz"
SRC_URI[md5sum] = "62b25cedcaabb8902bf945228be3bd4e"
SRC_URI[sha1sum] = "1ded0786e38a66c98b0727244c38ffb7341476ce"
SRC_URI[sha256sum] = "a95444091ec008ce87c3f74d614c7b9ed251f3a12df4b8e5cc0d119a0c08c89b"
SRC_URI[sha384sum] = "3b57c7073efbe0870db8f7faa1ad859261407b63923d041d8f9d9b6ffbb605b399d3444743fb63ada401f872f49c7f8f"
SRC_URI[sha512sum] = "bc9ec802cdd722a3577c246a94bb8057632456a3d78a45408c3d77bd27f9bce7f0a4c6dca532791297252073bbd738ab23fd5e9ba0315d346d0746bb85714873"

S = "${WORKDIR}/hello-${PV}"

# NOTE: if this software is not capable of being built in a separate build directory
# from the source, you should replace autotools with autotools-brokensep in the
# inherit line
inherit gettext autotools

# Specify any options you want to pass to the configure script using EXTRA_OECONF:
EXTRA_OECONF = ""

