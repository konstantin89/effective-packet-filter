obj-m += filter_module.o
filter_module-objs := Module.o CharDevice.o Status.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

i:
	sudo insmod filter_module.ko

u:
	sudo rmmod filter_module.ko

log:
	sudo dmesg -w

ls:
	sudo lsmod