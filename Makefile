obj-m += leap.o

KDIR := /lib/modules/$(shell uname -r)/build
#KDIR := /usr/src/linux-headers-$(shell uname -r)
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	rm -f *.o *.ko *.mod.c Module.symvers modules.order
