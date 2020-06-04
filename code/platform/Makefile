 #
 # @Author: cpu_code
 # @Date: 2020-06-03 20:25:29
 # @LastEditTime: 2020-06-04 19:38:01
 # @FilePath: \Linux_driver\code\platform\Makefile
 # @Gitee: https://gitee.com/cpu_code
 # @CSDN: https://blog.csdn.net/qq_44226094
 #

KERNELDIR := /home/cpucode/linux
CURRENT_PATH := $(shell pwd)

obj-m := leddevice.o
obj-m += leddriver.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean