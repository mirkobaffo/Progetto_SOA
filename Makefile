obj-m += ModuleManagement.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	
mount:
	sudo insmod ModuleManagement.ko
	
remove:
	sudo rmmod ModuleManagement.ko 
