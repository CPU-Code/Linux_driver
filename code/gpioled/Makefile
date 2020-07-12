/*
 * @Author: cpu_code
 * @Date: 2020-05-19 22:40:41
 * @LastEditTime: 2020-05-20 10:43:34
 * @FilePath: \Linux_driver\6gpioled\Makefile
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */ 

KERNELDIR := /home/cpucode/linux

CURRENT_PATH := $(shell pwd)

obj-m := gpioled.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean