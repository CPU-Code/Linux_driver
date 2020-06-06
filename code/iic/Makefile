/*
 * @Author: cpu_code
 * @Date: 2020-06-05 22:52:06
 * @LastEditTime: 2020-06-06 18:55:45
 * @FilePath: \Linux_driver\code\iic\Makefile
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */ 
KERNELDIR := /home/cpucode/linux
CURRENT_PATH := $(shell pwd)

obj-m := ap3216c.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean