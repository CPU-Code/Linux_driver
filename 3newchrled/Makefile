 #
 # @Author: cpu_code
 # @Date: 2020-05-17 18:05:32
 # @LastEditTime: 2020-05-17 19:54:11
 # @FilePath: \Linux_driver\3newchrled\Makefile
 # @Gitee: https://gitee.com/cpu_code
 # @CSDN: https://blog.csdn.net/qq_44226094
 #

KERNELDIR := /home/cpudcode/linux
CURRENT_PATH := $(shell pwd)

obj-m := newchrled.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean