 #
 # @Author: cpu_code
 # @Date: 2020-05-26 15:55:54
 # @LastEditTime: 2020-05-26 17:15:54
 # @FilePath: \Linux_driver\code\13timer\Makefile
 # @Gitee: https://gitee.com/cpu_code
 # @CSDN: https://blog.csdn.net/qq_44226094
 #
KERNELDIR := /home/cpucode/linux

CURRENT_PATH := $(shell pwd)

obj-m := timer.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean