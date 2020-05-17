 #
 # @Author: cpu_code
 # @Date: 2020-05-17 10:36:33
 # @LastEditTime: 2020-05-17 13:41:18
 # @FilePath: \Linux_driver\chrdevbase\Makefile
 # @Gitee: https://gitee.com/cpu_code
 # @CSDN: https://blog.csdn.net/qq_44226094
 #

# 开发板所使用的 Linux 内核源码目录，使用绝对路径
KERNELDIR := /home/cpucode/linux

# CURRENT_PATH 表示当前路径，直接通过运行“pwd”命令来获取当前所处路径
CURRENT_PATH := $(shell pwd)

# 将 chrdevbase.c 这个文件编译为 chrdevbase.ko 模块
obj-m := chrdevbase.o

build: kernel_modules

# modules 表示编译模块
#  -C 表示将当前的工作目录切换到指定目录中，也就是 KERNERLDIR 目录
# M 表示模块源码目录，“make modules”命令中加入 M=dir 
# 以后程序会自动到指定的 dir 目录中读取模块的源码并将其编译为.ko 文件
kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean