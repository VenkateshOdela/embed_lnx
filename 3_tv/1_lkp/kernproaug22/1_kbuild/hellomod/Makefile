#target command 
#input file hello.o generate hello.ko
#list input files here then generate .ko
obj-m:=hello.o

#variables 
#declaring path where scripts files are present
KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

# CONFIG_MODULE_SIG=n
# command to convert input files into desired target 
# this command uses kbuild scripts of kernel-header/source folder specified
build:
#make : -C means change diectory(where rules are defined) 
#input files are present in PWD 
#one more instance of make is invoking here
#This is Kbuid Make invoked by command line make
#Kbuild make will read input files obj-m=hell.o
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

#build clean are target names

# $ make build
# $ make clean
# $ make
#   - when no target mentioned , it will run the primary target in the file i.e build

# input file hello.c
# output files : hello.mod.c , hello.mod , hello.mod.o , hello.o
#
#
#
#

