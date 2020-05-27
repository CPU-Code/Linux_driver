 #
 # @Author: cpu_code
 # @Date: 2020-05-23 22:54:21
 # @LastEditTime: 2020-05-24 12:05:22
 # @FilePath: \Linux_driver\7beep\Makefile
 # @Gitee: https://gitee.com/cpu_code
 # @CSDN: https://blog.csdn.net/qq_44226094
 #

KERNELDIR := /home/cpucode/cpucodefile/linux-5.4.x

CURRENT_PATH := $(shell pwd)

obj-m := beep.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean