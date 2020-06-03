# Linux_driver

## 介绍
Linux驱动开发

## 文件说明


inlcude： 
    Linux内核文件

    include:
        linux:
            of.h：设备树节点等操作
            device.h：类
            cdev.h：字符设备
            fs.h: 设备号
            of_gpio.h
            atomic_fallback.h
            gpio.h：中断号
            kdev_t.h
            of_fdt.h
            types.h
            uaccess.h
            delay.h
            jiffies.h
            mutex.h
            rwlock_types.h
            rwlock.h
            semaphore.h
            seqlock.h
            spinlock_types.h
            spinlock.h
            timer.h
            interrupt.h：中断操作、tasklet
            irqflags.h 中断操作
            irqreturn.h：中断类型
            workqueue.h: 工作
            irqdomain.h: 
            poll.h：非堵塞
            wait.h: 等待队列头

        asm-generic:
            bitops:
                atomic.h
            gpio.h
            atomic-instrumented.h
            atomic.h
            param.h
            current.h: 当前进程

    arch
        arm:
            boot\dts: 设备树
                imx6ull.dtsi
                inmx6ull-colibri.dtsi
                Makefile
                imx6ul.dtsi：中断设备
                imx6ull-colibri-eval-v3.dtsi: 中断设备

            include: 头文件

            include/asm:
                mach:
                    arch.h

                gpio.h：中断
                atomic.h
                delay.h

            mach-imx：
                mach-imx6ul.c

            kernel：
                setup.c
                devtrre.c

            drivers\of:
                fdt.c

    kernel:
        softirq.c: 软中断
        workqueue_internal.h: 工作线程
        workqueue.c: 工作队列
        irq:
            manage.c: 释放掉相应的中断
        sched:
            wait.c: 等待队列项添加


    drivers/of:
        irq.c: 设备号

    Documentation\devicetree\bindings: 绑定文档，设备树是用来描述板子上的设备信息的，不同的设备其信息不同，反映到设备树中就是属性不同

    scripts\dtc: dtc工具
        Makefile

uboot_include： 
    uboot的文件

    common:
        fdt_support.c：chosen 节点的 bootargs 参数 生成

code:

    1chrdevbase：字符驱动开发的模板

    2Led：LED驱动开发

    3newchrled：新LED字符驱动开发

    4dtsled:

    5dtsled:

    6gpioled:

    7beep:

    8atomic:

    9spinlock:

    10

    11

    12

    13

    14irq：中断实验

    block: 堵塞IO

    noblock: 非堵塞

    asyncnoti: 异步通知

## 使用说明

#### 初次编译
xxx.dts 是设备树源码文件，编译 Linux 的时候会将其编译为xxx.dtb 文件

设备树：arch/arm/boot/dts/Makefile ， 找 到 “ dtb-$(CONFIG_SOC_XXX) ”配置项( 什么芯片就放在那个配置项 )，在此配置项中加入“xxx.dtb”

设备树编写完成以后使用命令重新编译设备树
```shell
make dtbs
```
使用编译出来的 xxx.dtb 文件启动 Linux 系统


#### 程序编译：
输入如下命令编译出驱动模块文件：
```shell
make
```
编译成功以后就会生成一个名为“xxx.ko”的驱动模块文件

输入如下命令编译测试 xxxAPP.c 这个测试程序
```shell
arm-linux-gnueabihf-gcc xxxAPP.c -o xxxApp
```
编译成功以后就会生成 xxxApp 这个应用程序

输入如下命令查看xxxAPP 这个程序的文件信息
```shell
file xxxApp
```

编译出来的 xxx.ko 和 xxxApp 这两个文件拷贝到 指定 目录中
```shell
sudo cp xxx.ko xxxApp /home/xxx -f
```

重启开发板，进入到指定的目录
```shell
cd /home/xxx
```

第一次加载驱动的时候需要运行此命令
```shell
depmod
```
驱动加载成功以后创建“/dev/xxx”设备节点

使用 modprobe 加载 xxx.ko
```shell
modprobe xxx.ko
```

输入“ lsmod ”命令即可查看当前系统中存在的模块
```shell
lsmod
```

创建设备节点文件
```shell
mknod /dev/xxx 字符设备 主设备号 次设备号
```

设备操作测试
```shell
./xxxApp /dev/xxx 参数
```

卸载驱动模块
```shell
rmmod xxx.ko
```


## 错误

注意 file_operations 是否在这些函数下面

```shell
make -C /home/cpucode/cpucodefile/linux-5.5.7/linux-5.5.7 M=/home/cpucode/cpucodefile/drivers/led modules
make[1]: Entering directory '/home/cpucode/cpucodefile/linux-5.5.7/linux-5.5.7'
  CC [M]  /home/cpucode/cpucodefile/drivers/led/led.o
/home/cpucode/cpucodefile/drivers/led/led.c:44:10: error: ‘led_open’ undeclared here (not in a function); did you mean ‘seq_open’?
  .open = led_open,
          ^~~~~~~~
          seq_open
/home/cpucode/cpucodefile/drivers/led/led.c:45:10: error: ‘led_read’ undeclared here (not in a function); did you mean ‘seq_read’?
  .read = led_read,
          ^~~~~~~~
          seq_read
/home/cpucode/cpucodefile/drivers/led/led.c:46:11: error: ‘led_write’ undeclared here (not in a function); did you mean ‘seq_write’?
  .write = led_write,
           ^~~~~~~~~
           seq_write
/home/cpucode/cpucodefile/drivers/led/led.c:47:14: error: ‘led_release’ undeclared here (not in a function); did you mean ‘seq_release’?
  .release =  led_release,
              ^~~~~~~~~~~
              seq_release
/home/cpucode/cpucodefile/drivers/led/led.c:181:12: warning: ‘led_release’ defined but not used [-Wunused-function]
 static int led_release(struct inode *inode, struct file *filp)
            ^~~~~~~~~~~
/home/cpucode/cpucodefile/drivers/led/led.c:145:16: warning: ‘led_write’ defined but not used [-Wunused-function]
 static ssize_t led_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
                ^~~~~~~~~
/home/cpucode/cpucodefile/drivers/led/led.c:127:16: warning: ‘led_read’ defined but not used [-Wunused-function]
 static ssize_t led_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
                ^~~~~~~~
/home/cpucode/cpucodefile/drivers/led/led.c:109:12: warning: ‘led_open’ defined but not used [-Wunused-function]
 static int led_open(struct inode *inode, struct file *filp)
            ^~~~~~~~
scripts/Makefile.build:265: recipe for target '/home/cpucode/cpucodefile/drivers/led/led.o' failed
make[2]: *** [/home/cpucode/cpucodefile/drivers/led/led.o] Error 1
Makefile:1694: recipe for target '/home/cpucode/cpucodefile/drivers/led' failed
make[1]: *** [/home/cpucode/cpucodefile/drivers/led] Error 2
make[1]: Leaving directory '/home/cpucode/cpucodefile/linux-5.5.7/linux-5.5.7'
Makefile:19: recipe for target 'kernel_modules' failed
make: *** [kernel_modules] Error 2
```


```shell
make -C /home/cpucode/cpucodefile/linux-4.1.15/linux-imx-rel_imx_4.1.15_2.1.0_ga M=/home/cpucode/cpucodefile/drivers/led modules
make[1]: Entering directory '/home/cpucode/cpucodefile/linux-4.1.15/linux-imx-rel_imx_4.1.15_2.1.0_ga'
  CC [M]  /home/cpucode/cpucodefile/drivers/led/led.o
cc1: error: code model kernel does not support PIC mode
scripts/Makefile.build:264: recipe for target '/home/cpucode/cpucodefile/drivers/led/led.o' failed
make[2]: *** [/home/cpucode/cpucodefile/drivers/led/led.o] Error 1
Makefile:1384: recipe for target '_module_/home/cpucode/cpucodefile/drivers/led' failed
make[1]: *** [_module_/home/cpucode/cpucodefile/drivers/led] Error 2
make[1]: Leaving directory '/home/cpucode/cpucodefile/linux-4.1.15/linux-imx-rel_imx_4.1.15_2.1.0_ga'
Makefile:19: recipe for target 'kernel_modules' failed
make: *** [kernel_modules] Error 2
```

```shell
make -C /home/cpucode/cpucodefile/linux-5.5.7/linux-5.5.7 M=/home/cpucode/cpucodefile/drivers/led modules
make[1]: Entering directory '/home/cpucode/cpucodefile/linux-5.5.7/linux-5.5.7'
  CC [M]  /home/cpucode/cpucodefile/drivers/led/led.o
In file included from ./include/linux/types.h:6:0,
                 from /home/cpucode/cpucodefile/drivers/led/led.c:9:
./include/uapi/linux/types.h:5:10: fatal error: asm/types.h: No such file or directory
 #include <asm/types.h>
          ^~~~~~~~~~~~~
compilation terminated.
scripts/Makefile.build:265: recipe for target '/home/cpucode/cpucodefile/drivers/led/led.o' failed
make[2]: *** [/home/cpucode/cpucodefile/drivers/led/led.o] Error 1
Makefile:1693: recipe for target '/home/cpucode/cpucodefile/drivers/led' failed
make[1]: *** [/home/cpucode/cpucodefile/drivers/led] Error 2
make[1]: Leaving directory '/home/cpucode/cpucodefile/linux-5.5.7/linux-5.5.7'
Makefile:19: recipe for target 'kernel_modules' failed
make: *** [kernel_modules] Error 2
```


```shell
make -C /home/cpucode/cpucodefile/linux-5.5.7 M=/home/cpucode/cpucodefile/drivers/led modules
make[1]: Entering directory '/home/cpucode/cpucodefile/linux-5.5.7'
make[1]: *** No rule to make target 'modules'.  Stop.
make[1]: Leaving directory '/home/cpucode/cpucodefile/linux-5.5.7'
Makefile:19: recipe for target 'kernel_modules' failed
make: *** [kernel_modules] Error 2
```

```shell
make -C /home/cpucode/linux-5.5.7 M=/home/cpucode/cpucodefile/drivers/led modules
make[1]: *** /home/cpucode/linux-5.5.7: No such file or directory.  Stop.
Makefile:19: recipe for target 'kernel_modules' failed
make: *** [kernel_modules] Error 2
```