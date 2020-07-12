<!--
 * @Author: cpu_code
 * @Date: 2020-07-08 21:13:40
 * @LastEditTime: 2020-07-12 13:06:22
 * @FilePath: \Linux_driver\README.md
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
--> 

# Linux_driver

 * @Author: cpu_code
 * @Date: 2020-07-08 21:13:40
 * @LastEditTime: 2020-07-08 21:15:27
 * @FilePath: \Linux_driver\README.md
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094


## 介绍
Linux驱动开发

--------------

## 文件说明

* [inlcude__Linux内核文件](#inlcude__Linux内核文件)

* [code__驱动代码](#code__驱动代码)

----------------------------------

## [inlcude__Linux内核文件](inlcude)

### [include](inlcude/inlcude)

#### [linux](inlcude/inlcude/linux)

* [of.h__设备树节点等操作](inlcude/inlcude/linux/of.h)

* [device.h__类、总线](inlcude/inlcude/linux/device.h)

* [cdev.h__字符设备](inlcude/inlcude/linux/cdev.h)

* [fs.h__设备号](inlcude/inlcude/linux/fs.h)

* [of_gpio.h](inlcude/inlcude/linux/of_gpio.h)

* [atomic_fallback.h](inlcude/inlcude/linux/atomic_fallback.h)

* [gpio.h__中断号](inlcude/inlcude/linux/gpio.h)

* [device.h__类、总线](inlcude/inlcude/linux/device.h)

* [device.h__类、总线](inlcude/inlcude/linux/device.h)

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
            mod_devicetable.h: compatible比较
            platform_device.h: platform 设备
            miscdevice.h: MISC 设备
            input.h: input设备
            i2c.h: iic传输函数接口

#### [asm-generic](inlcude/inlcude/asm-generic)

            bitops:
                atomic.h
            gpio.h
            atomic-instrumented.h
            atomic.h
            param.h
            current.h: 当前进程

#### [uapi](inlcude/inlcude/uapi)

            linux：
                input-event-codes.h： 可选的事件类型
                major.h:  input 子系统的所有设备主设备号都为 13 
                input.h:  表示所有的输入事件
                time.h:  时间
                i2c.h: 处理初始信息

### [arch](inlcude/arch)

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
                mach-armadillo5x0.c：

            kernel：
                setup.c
                devtrre.c

            drivers\of:
                fdt.c

### [kernel](inlcude/kernel)

        softirq.c: 软中断
        workqueue_internal.h: 工作线程
        workqueue.c: 工作队列
        irq:
            manage.c: 释放掉相应的中断
        sched:
            wait.c: 等待队列项添加

### [drivers](inlcude/drivers)
    /of:
        irq.c: 设备号

    Documentation\devicetree\bindings: 绑定文档，设备树是用来描述板子上的设备信息的，不同的设备其信息不同，反映到设备树中就是属性不同

### [scripts](inlcude/scripts)
    \dtc: dtc工具
        Makefile

-------------------------

## [uboot_include__uboot的文件](uboot_include)

### [common](uboot_include/common)

* [fdt_support.c__chosen 节点的 bootargs 参数 生成](uboot_include/common/fdt_support.c)


------------------------------------

## [code__驱动代码](code)

* [chrdevbase__字符驱动开发的模板](code/chrdevbase)

* [Led__LED驱动开发](code/Led)

* [newchrled__新LED字符驱动开发](code/newchrled)

* [mydts__设备树](code/mydts)

* [dtsled__](code/dtsled)

* [gpioled__](code/gpioled)

* [beep__](code/beep)

* [atomic__](code/atomic)

* [spinlock__](code/spinlock)

* [semaphore__](code/semaphore)

* [mutex__](code/mutex)

* [key__](code/key)

* [timer__](code/timer)

* [irq__中断实验](code/irq)

* [block__堵塞IO](code/block)

* [noblock__非堵塞](code/noblock)

* [asyncnoti__异步通知](code/asyncnoti)

* [platform__platform设备驱动](code/platform)

* [dtsplatform__设备树platform](code/dtsplatform)

* [miscbeep__蜂鸣器杂项驱动](code/miscbeep)

* [input__input子系统](code/input)

* [iic__陀螺仪IIC协议](code/iic)

* [spi__SPI协议](code/spi)


-----------------------------


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

-----------------------------

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