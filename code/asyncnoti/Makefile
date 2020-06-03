 #
 # @Author: cpu_code
 # @Date: 2020-06-03 10:10:13
 # @LastEditTime: 2020-06-03 17:31:21
 # @FilePath: \Linux_driver\code\asyncnoti\Makefile
 # @Gitee: https://gitee.com/cpu_code
 # @CSDN: https://blog.csdn.net/qq_44226094
 #

KERNELDIR := /home/cpucode/linux
CURRENT_PATH := $(shell pwd)

obj-m := asyncnoti.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean