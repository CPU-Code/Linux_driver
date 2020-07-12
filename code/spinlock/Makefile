 #
 # @Author: cpu_code
 # @Date: 2020-05-24 18:04:54
 # @LastEditTime: 2020-05-25 16:27:12
 # @FilePath: \Linux_driver\9spinlock\Makefile
 # @Gitee: https://gitee.com/cpu_code
 # @CSDN: https://blog.csdn.net/qq_44226094
 #

KERNELDIR := /home/cpucode/linux
CURRENT_PATH := $(shell pwd)

obj-m := spinlock.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean