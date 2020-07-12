 #
 # @Author: cpu_code
 # @Date: 2020-05-25 17:43:29
 # @LastEditTime: 2020-05-25 21:16:39
 # @FilePath: \Linux_driver\10semaphore\Makefile
 # @Gitee: https://gitee.com/cpu_code
 # @CSDN: https://blog.csdn.net/qq_44226094
 #

KERNELDIR := /home/cpucode/linux
CURRENT_PATH := $(shell pwd)

obj-m := semaphore.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean