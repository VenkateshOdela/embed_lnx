# kbuild part of makefile
ifneq ($(KERNELRELEASE),)
#obj-m := kthread.o
obj-m += kthread_at.o
#obj-m += kthread_spin.o
#obj-m += kthread_sync_waitq.o
#obj-m += kthread_pcpu.o
EXTRA_CFLAGS += -DDEBUG
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
