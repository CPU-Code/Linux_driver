/*
 * @Author: cpu_code
 * @Date: 2020-06-03 20:24:20
 * @LastEditTime: 2020-06-03 21:11:27
 * @FilePath: \Linux_driver\code\platform\leddevice.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */ 
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of_gpio.h>
#include <linux/semaphore.h>
#include <linux/timer.h>
#include <linux/irq.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/platform_device.h>

#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>

/**
 * 寄存器地址定义
 */
#define CCM_CCGR1_BASE				(0X020C406C)
#define SW_MUX_GPIO1_IO03_BASE		(0X020E0068)
#define SW_PAD_GPIO1_IO03_BASE		(0X020E02F4)
#define GPIO1_DR_BASE				(0X0209C000)
#define GPIO1_GDIR_BASE				(0X0209C004)
#define REGISTER_LENGTH				4

/**
 * @function: 释放flatform设备模块的时候此函数会执行
 * @parameter: 
 *      dev 	: 要释放的设备
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static void	led_release(struct device *dev)
{
    printk("led device released!\r\n");
}

/**
 * @function: 备资源信息，也就是LED0所使用的所有寄存器
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static struct resource led_resources[] = {
    [0] = {
        .start 	= CCM_CCGR1_BASE,
        .end 	= (CCM_CCGR1_BASE + REGISTER_LENGTH - 1),
        .flags 	= IORESOURCE_MEM,
    },
    [1] = {
        .start	= SW_MUX_GPIO1_IO03_BASE,
        .end	= (SW_MUX_GPIO1_IO03_BASE + REGISTER_LENGTH - 1),
        .flags	= IORESOURCE_MEM,
    },
    [2] = {
        .start	= SW_PAD_GPIO1_IO03_BASE,
        .end	= (SW_PAD_GPIO1_IO03_BASE + REGISTER_LENGTH - 1),
        .flags	= IORESOURCE_MEM,
    },
    [3] = {
        .start	= GPIO1_DR_BASE,
        .end	= (GPIO1_DR_BASE + REGISTER_LENGTH - 1),
        .flags	= IORESOURCE_MEM,
    },
    [4] = {
        .start	= GPIO1_GDIR_BASE,
        .end	= (GPIO1_GDIR_BASE + REGISTER_LENGTH - 1),
        .flags	= IORESOURCE_MEM,
    },
};

/**
 * platform设备结构体 
 */
static struct platform_device leddevice = {
    .name = "led",
    .id = -1,
    .dev = {
        .release = &led_release,
    },
    .num_resources = ARRAY_SIZE(led_resources),
    .resource = led_resources,
};

/**
 * @function: 设备模块加载
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static int __init leddevice_init(void)
{
    // 向 Linux 内核注册 platform 设备
    return platform_device_register(&leddevice);
}

/**
 * @function: 设备模块注销
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static void __exit leddevice_exit(void)
{
    //从 Linux 内核中删除掉 platform 设备
    platform_device_unregister(&leddevice);
}

module_init(leddevice_init);
module_exit(leddevice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("cpucode");