/*
 * @Author: cpu_code
 * @Date: 2020-06-06 21:02:07
 * @LastEditTime: 2020-06-07 12:32:23
 * @FilePath: \Linux_driver\code\spi\Makefile
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */ 
KERNELDIR := /home/cpudcode/linux
CURRENT_PATH := $(shell pwd)

obj-m := icm20608.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean