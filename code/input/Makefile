 #
 # @Author: cpu_code
 # @Date: 2020-06-05 09:52:57
 # @LastEditTime: 2020-06-05 17:33:15
 # @FilePath: \Linux_driver\code\input\Makefile
 # @Gitee: https://gitee.com/cpu_code
 # @CSDN: https://blog.csdn.net/qq_44226094
 #
 
KERNELDIR := /home/cpucode/linux
CURRENT_PATH := $(shell pwd)

obj-m := keyinput.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean