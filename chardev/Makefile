CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)

EXTRA_CFLAGS += -std=gnu99

TARGET1 = chardev_2_0

obj-m	:= $(TARGET1).o 

all: default clean

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules


clean:	
	@rm -f *.o .*.cmd .*.flags *.mod.c *.order
	@rm -f .*.*.cmd *.symvers *~ *.*~
	@rm -fR .tmp*
	@rm -rf .tmp_versions

disclean: clean	
	@rm -f *.ko mcat
