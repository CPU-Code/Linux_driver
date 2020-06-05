 #
 # @Author: cpu_code
 # @Date: 2020-06-04 22:57:25
 # @LastEditTime: 2020-06-05 09:37:48
 # @FilePath: \Linux_driver\code\miscbeep\Makefile
 # @Gitee: https://gitee.com/cpu_code
 # @CSDN: https://blog.csdn.net/qq_44226094
 #

KERNELDIR := /home/cpucode/linux
CURRENT_PATH := $(shell pwd)

obj-m := miscbeep.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean