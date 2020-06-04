 #
 # @Author: cpu_code
 # @Date: 2020-06-04 20:52:43
 # @LastEditTime: 2020-06-04 21:19:13
 # @FilePath: \Linux_driver\code\dtsplatform\Makefile
 # @Gitee: https://gitee.com/cpu_code
 # @CSDN: https://blog.csdn.net/qq_44226094
 #

KERNELDIR := /home/cpucode/linux
CURRENT_PATH := $(shell pwd)

obj-m := leddriver.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean