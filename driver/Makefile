obj-m += filter_module.o

UTILS_FILES = utils/Status.o
HOOKS_FILES = hooks/HooksManager.o

filter_module-objs := Module.o CharDevice.o DeviceClassManager.o $(UTILS_FILES) $(HOOKS_FILES)

EXTRA_CFLAGS += -I$(PWD)

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