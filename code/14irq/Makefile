 #
 # @Author: cpu_code
 # @Date: 2020-05-27 14:13:00
 # @LastEditTime: 2020-05-28 11:43:39
 # @FilePath: \Linux_driver\code\14irq\Makefile
 # @Gitee: https://gitee.com/cpu_code
 # @CSDN: https://blog.csdn.net/qq_44226094
 #
 
KERNELDIR := /home/cpucode/linux
CURRENT_PATH := $(shell pwd)

obj-m := irq.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean