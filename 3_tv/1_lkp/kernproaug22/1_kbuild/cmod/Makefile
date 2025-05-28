
# 1.This syntax of makefile is used when bunch of .c files are used to generate one ko

# 3 input files hello.c, var.c , pro.c
# mymod.o, mymod.c ? is not present
# we have generate mymod.o from hello.o var.o pro.o(link)
# bunch of .c files -> to create ko -> is posible.
 
# This makefile as two parts with conditional statement KERNELRELEASE
# one part is used for commnadline makefile
# other part is used for kbuid makefile; 

# other way, Kbuild file with inputs of Kbuild makefile can also be placed in M=pwd. 


# kbuild part of makefile
ifneq ($(KERNELRELEASE),)
obj-m := mymod.o
mymod-y := hello.o var.o pro.o
else

KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

# CONFIG_MODULE_SIG=n
# command to convert input files into desired target 
# this command uses kbuild scripts of kernel-header/source folder specified
build:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

endif
