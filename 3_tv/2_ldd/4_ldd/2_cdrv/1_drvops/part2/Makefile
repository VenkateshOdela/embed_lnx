obj-m  := vdevs_platform.o vdevs_drv.o

KDIR= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD)  modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -rf $(wildcard *.o *.ko *.mo* Mo* *.order)
	

