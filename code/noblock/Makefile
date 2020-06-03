/*
 * @Author: cpu_code
 * @Date: 2020-06-02 19:25:01
 * @LastEditTime: 2020-06-02 22:01:56
 * @FilePath: \Linux_driver\code\noblock\Makefile
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */ 
KERNELDIR := /home/cpucode/linux
CURRENT_PATH := $(shell pwd)

obj-m := noblock.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
