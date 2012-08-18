obj-m := modul.o
KDIR = /lib/modules/$(shell uname -r)/build
all:
	make -C /usr/src/linux-headers-2.6.32-41-generic-tuxonice SUBDIRS=${PWD}

clean:
	rm -f *.o *.tmp .*.o.* .tmp_versions/* .*.ko.cmd *.mod.c Module.symvers modules.order
	rmdir .tmp_versions



