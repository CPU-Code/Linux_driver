 #
 # @Author: cpu_code
 # @Date: 2020-05-18 12:30:52
 # @LastEditTime: 2020-05-18 18:09:10
 # @FilePath: \Linux_driver\4dtsled\Makefile
 # @Gitee: https://gitee.com/cpu_code
 # @CSDN: https://blog.csdn.net/qq_44226094
 #

KERNELDIR := /home/cpucode/linux
CURRENT_PATH := $(shell pwd)

obj-m := dtsled.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean