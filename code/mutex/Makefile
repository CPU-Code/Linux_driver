 #
 # @Author: cpu_code
 # @Date: 2020-05-26 09:14:27
 # @LastEditTime: 2020-05-26 10:22:30
 # @FilePath: \Linux_driver\code\11mutex\Makefile
 # @Gitee: https://gitee.com/cpu_code
 # @CSDN: https://blog.csdn.net/qq_44226094
 #

KERNELDIR := /home/cpucode/linux

CURRENT_PATH := $(shell pwd)

obj-m := mutex.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean