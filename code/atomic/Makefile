 #
 # @Author: cpu_code
 # @Date: 2020-05-24 12:30:46
 # @LastEditTime: 2020-05-24 16:26:26
 # @FilePath: \Linux_driver\8atomic\Makefile
 # @Gitee: https://gitee.com/cpu_code
 # @CSDN: https://blog.csdn.net/qq_44226094
 #

KERNELDIR := /home/cpucode/linux
CURRENT_PATH := $(shell pwd)

obj-m := atomic.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean