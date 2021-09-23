obj-m += syscall_table_discoverer.o 

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	
mount:
	sudo insmod syscall_table_discoverer.ko
	
remove:
	sudo rmmod syscall_table_discoverer.ko 
