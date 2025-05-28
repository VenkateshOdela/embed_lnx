Linux Kernel Build subsystem
----------------------------

> #### major components of kbuild:

```shell
Makefile				the top Makefile.
.config					the kernel configuration file.
arch/$(ARCH)/Makefile			the arch Makefile.
scripts/Makefile.*			common rules etc. for all kbuild Makefiles.
kbuild Makefiles			Local Makefiles located in subfolders of source tree.
Kconfig					kernel config menu scripts
```

> #### Build Summary:
>

- The top Makefile reads the .config file, which comes from the kernel  configuration process. The top Makefile is responsible for building two major products: vmlinux  (the resident/raw kernel image) and modules (.ko files).It builds these goals by recursively descending into the subdirectories of  the kernelsource tree.
  
- The list of subdirectories visited depends upon the kernel configuration. Each subdirectory has a kbuild Makefile which carries out the commands passed down from above. The kbuild Makefile uses information from the .config file to construct 
  various file lists used by kbuild to build any built-in(statically linked)  or modular targets(module .ko).

- scripts/Makefile.* contains all the definitions/rules that are used to execute build commands present in Kbuild Makefiles.
  for example: commands to run for "obj-y" or "obj-m" targets. The top Makefile textually includes an arch Makefile with the name arch/$(ARCH)/Makefile. The arch Makefile supplies architecture-specific information to create the loadable kernel image.
	

> #### kbuild Makefile Goal definitions

```shell
Goal definitions are core components of the kbuild Makefile.
These lines define the files to be built, any special compilation
options, and any subdirectories to be entered recursively.

The most simple kbuild makefile contains one line:

Example:
	obj-y += foo.o

This tells kbuild that there is one object in that directory, named
foo.o. foo.o will be built from foo.c or foo.S.

If foo.o shall be built as a module, the variable obj-m is used.
Therefore the following pattern is often used:

Example:
	obj-$(CONFIG_FOO) += foo.o

$(CONFIG_FOO) evaluates to either y (for built-in) or m (for module).
If CONFIG_FOO is neither y nor m, then the file will not be compiled
nor linked.
```

--- 3.2 Built-in object goals - obj-y

```shell
The kbuild Makefile specifies object files for vmlinux
in the $(obj-y) lists.  These lists depend on the kernel
configuration.

Kbuild compiles all the $(obj-y) files.  It then calls
"$(LD) -r" to merge these files into one built-in.o file.
built-in.o is later linked into vmlinux by the parent Makefile.

The order of files in $(obj-y) is significant.  Duplicates in
the lists are allowed: the first instance will be linked into
built-in.o and succeeding instances will be ignored.

Link order is significant, because certain functions
(module_init() / __initcall) will be called during boot in the
order they appear. So keep in mind that changing the link
order may e.g. change the order in which your SCSI
controllers are detected, and thus your disks are renumbered.

Example:
	#drivers/isdn/i4l/Makefile
	# Makefile for the kernel ISDN subsystem and device drivers.
	# Each configuration option enables a list of files.
	obj-$(CONFIG_ISDN_I4L)         += isdn.o
	obj-$(CONFIG_ISDN_PPP_BSDCOMP) += isdn_bsdcomp.o
```

--- 3.3 Loadable module goals - obj-m

```shell
$(obj-m) specify object files which are built as loadable
kernel modules.

A module may be built from one source file or several source
files. In the case of one source file, the kbuild makefile
simply adds the file to $(obj-m).

Example:
	#drivers/isdn/i4l/Makefile
	obj-$(CONFIG_ISDN_PPP_BSDCOMP) += isdn_bsdcomp.o

Note: In this example $(CONFIG_ISDN_PPP_BSDCOMP) evaluates to 'm'

If a kernel module is built from several source files, you specify
that you want to build a module in the same way as above; however,
kbuild needs to know which object files you want to build your
module from, so you have to tell it by setting a $(<module_name>-y)
variable.

Example:
	#drivers/isdn/i4l/Makefile
	obj-$(CONFIG_ISDN_I4L) += isdn.o
	isdn-y := isdn_net_lib.o isdn_v110.o isdn_common.o

In this example, the module name will be isdn.o. Kbuild will
compile the objects listed in $(isdn-y) and then run
"$(LD) -r" on the list of these files to generate isdn.o.

Due to kbuild recognizing $(<module_name>-y) for composite objects,
you can use the value of a CONFIG_ symbol to optionally include an
object file as part of a composite object.

Example:
	#fs/ext2/Makefile
        obj-$(CONFIG_EXT2_FS) += ext2.o
	ext2-y := balloc.o dir.o file.o ialloc.o inode.o ioctl.o \
		  namei.o super.o symlink.o
        ext2-$(CONFIG_EXT2_FS_XATTR) += xattr.o xattr_user.o \
					xattr_trusted.o

In this example, xattr.o, xattr_user.o and xattr_trusted.o are only
part of the composite object ext2.o if $(CONFIG_EXT2_FS_XATTR)
evaluates to 'y'.

Note: Of course, when you are building objects into the kernel,
the syntax above will also work. So, if you have CONFIG_EXT2_FS=y,
kbuild will build an ext2.o file for you out of the individual
parts and then link this into built-in.o, as you would expect.
```

