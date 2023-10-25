obj-m += filter_module.o
filter_module-objs := Module.o CharDevice.o Status.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean