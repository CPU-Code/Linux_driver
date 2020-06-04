/*
 * @Author: cpu_code
 * @Date: 2020-06-04 20:53:04
 * @LastEditTime: 2020-06-04 22:20:17
 * @FilePath: \Linux_driver\code\dtsplatform\leddriver.c
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

#define LEDDEV_CNT		1				/* 设备号长度 	*/
#define LEDDEV_NAME		"dtsplatled"	/* 设备名字 	*/
#define LEDOFF 			0
#define LEDON 			1

/* leddev设备结构体 */
struct leddev_dev
{
    dev_t devid;				/* 设备号	*/
    struct cdev cdev;			/* cdev		*/
    struct class *class;		/* 类 		*/
    struct device *device;		/* 设备		*/
    int major;					/* 主设备号	*/
    struct device_node *node;	/* LED设备节点 */
    int led0;					/* LED灯GPIO标号 */
};

struct leddev_dev leddev; 		/* led设备 */

/**
 * @function: LED打开/关闭
 * @parameter: 
 *      sta 	: LEDON(0) 打开LED，LEDOFF(1) 关闭LED
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
void led0_switch(u8 sta)
{
    if(sta == LEDON )
    {
        gpio_set_value(leddev.led0, 0);
    }
    else if(sta == LEDOFF)
    {
        gpio_set_value(leddev.led0, 1);
    }
}

/**
 * @function: 打开设备
 * @parameter: 
 *      inode 	: 传递给驱动的inode
 *      filp 	: 设备文件，file结构体有个叫做private_data的成员变量
 * 					  一般在open的时候将private_data指向设备结构体。
 * @return: 
 *     success: 0
 *     error: 其他
 * @note: 
 */
static int led_open(struct inode *inode, struct file *filp)
{
    filp->private_data = &leddev; /* 设置私有数据  */

    return 0;
}

/**
 * @function: 向设备写数据
 * @parameter: 
 *      filp 	: 设备文件，表示打开的文件描述符
 *      buf 	: 要写给设备写入的数据
 *      cnt 	: 要写入的数据长度
 *      offt 	: 相对于文件首地址的偏移
 * @return: 
 *     success: 写入的字节数
 *     error: 负值
 * @note: 
 */
static ssize_t led_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
    int retvalue;
	unsigned char databuf[2];
	unsigned char ledstat;

    retvalue = copy_from_user(databuf, buf, cnt);
    if(retvalue < 0)
    {
        printk("kernel write failed!\r\n");
		return -EFAULT;
    }

    ledstat = databuf[0];
    if(ledstat == LEDON)
    {
        led0_switch(LEDON);
    }
    else if(ledstat == LEDOFF)
    {
        led0_switch(LEDOFF);
    }

    return 0;
}

/* 设备操作函数 */
static struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .open = led_open,
	.write = led_write,
};

/**
 * @function: flatform驱动的probe函数，当驱动与设备匹配以后此函数就会执行
 * @parameter: 
 *      dev 	: platform设备
 * @return: 
 *     success: 0
 *     error: 其他
 * @note: 
 */
static int led_probe(struct platform_device *dev)
{
    printk("led driver and device was matched!\r\n");

    /* 1、设置设备号 */
	if (leddev.major)
    {
        leddev.devid = MKDEV(leddev.major, 0);
        //注册设备号
        register_chrdev_region(leddev.devid, LEDDEV_CNT, LEDDEV_NAME);
    }
    else
    {
        //申请设备号
        alloc_chrdev_region(&leddev.devid, 0, LEDDEV_CNT, LEDDEV_NAME);
        leddev.major = MAJOR(leddev.devid);
    }

    /* 2、注册设备      */
	cdev_init(&leddev.cdev, &led_fops);
    //添加字符设备
    cdev_add(&leddev.cdev, leddev.devid, LEDDEV_CNT);

    /* 3、创建类      */
	leddev.class = class_create(THIS_MODULE, LEDDEV_NAME);
    if(IS_ERR(leddev.class))
    {
        return PTR_ERR(leddev.class);
    }

    /* 4、创建设备 */
	leddev.device = device_create(leddev.class, NULL, leddev.devid, NULL, LEDDEV_NAME);
    if(IS_ERR(leddev.device))
    {
        return PTR_ERR(leddev.device);
    }

    /* 5、初始化IO */	
	leddev.node = of_find_node_by_path("/gpioled");
    if(leddev.node == NULL)
    {
        printk("gpioled node nost find!\r\n");
		return -EINVAL;
    }
    //获取 GPIO 编号
    leddev.led0 = of_get_named_gpio(leddev.node, "led-gpio", 0);
    if(leddev.led0 < 0)
    {
        printk("can't get led-gpio\r\n");
		return -EINVAL;
    }
    //申请GPIO
    gpio_request(leddev.led0, "led0");
    /* led0 IO设置为输出，默认高电平	*/
    gpio_direction_output(leddev.led0, 1);

    return 0;
}

/**
 * @function: platform驱动的remove函数，移除platform驱动的时候此函数会执行
 * @parameter: 
 *      dev 	: platform设备
 * @return: 
 *     success: 0
 *     error: 其他
 * @note: 
 */
static int led_remove(struct platform_device *dev)
{
    gpio_set_value(leddev.led0, 1); 	/* 卸载驱动的时候关闭LED */
    cdev_del(&leddev.cdev);				/*  删除cdev */
    unregister_chrdev_region(leddev.devid, LEDDEV_CNT); /* 注销设备号 */
    //删除设备
    device_destroy(leddev.class, leddev.devid);
    //删除类
    class_destroy(leddev.class);

    return 0;
}

/* 匹配列表 */
static const struct of_device_id led_of_match[] = {
    { .compatible = "gpioled" },    /* 兼容属性 */
    { /* Sentinel */ }  /* 最后一个元素一定要为空 */
};

/* platform驱动结构体 */
static struct platform_driver led_driver = {
    .driver		= {
        .name	= "led",			/* 驱动名字，用于和设备匹配 */
        .of_match_table	= led_of_match, /* 设备树匹配表 		 */
    },
    .probe		= led_probe,
    .remove		= led_remove,
};

/**
 * @function: 驱动模块加载函数
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static int __init leddriver_init(void)
{
    return platform_driver_register(&led_driver);
}

/**
 * @function: 驱动模块卸载函数
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static void __exit leddriver_exit(void)
{
    platform_driver_unregister(&led_driver);
}

module_init(leddriver_init);
module_exit(leddriver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("cpucode");