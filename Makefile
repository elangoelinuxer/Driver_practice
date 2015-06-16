obj-m +=simple_char_driver.o

all:
	make -C /lib/modules/$(shell uname -a)/build  M=$(PWD) modules

