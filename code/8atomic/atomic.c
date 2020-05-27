/*
 * @Author: cpu_code
 * @Date: 2020-05-24 12:30:18
 * @LastEditTime: 2020-05-24 17:57:26
 * @FilePath: \Linux_driver\8atomic\atomic.c
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
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>

#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#define GPIOLED_CNT			1		  	/* 设备号个数 */
#define GPIOLED_NAME		"gpioled"	/* 名字 */
#define LEDOFF 				0			/* 关灯 */
#define LEDON 				1			/* 开灯 */

/* gpioled设备结构体 */
struct gpioled_dev
{
    dev_t         devid;	 /* 设备号 	 */
    struct cdev   cdev;		 /* cdev 	*/
    struct class  *class; 	 /* 类 		*/
    struct device *device;	 /* 设备 	 */
    int           major;	 /* 主设备号	  */
    int           minor;	 /* 次设备号   */
    struct device_node	*nd; /* 设备节点 */
    int      led_gpio;		 /* led所使用的GPIO编号		*/
    atomic_t lock;			 /* 原子变量 */
};

struct gpioled_dev gpioled;	/* led设备 */

/**
 * @function: 打开设备
 * @parameter: 
 *      inode 	: 传递给驱动的inode
 *      filp 	: 设备文件，file结构体有个叫做private_data的成员变量
 *                  一般在open的时候将private_data指向设备结构体
 * @return: 
 *     success: 0
 *     error: 其他
 * @note: 
 */
static int led_open(struct inode *inode, struct file *filp)
{
    /* 通过判断原子变量的值来检查LED有没有被别的应用使用 */
    /* 每次判断减一 ， 为0就返回真*/
    if(!atomic_dec_and_test(&gpioled.lock))
    {
        /* 小于0的话就加1,使其原子变量等于0 */
        /* 每次进入就加一 */
        atomic_inc(&gpioled.lock);
        /* LED被使用，返回忙 */
        return -EBUSY;	
    }

    filp->private_data = &gpioled; /* 设置私有数据 */
    return 0;
}

/**
 * @function: 从设备读取数据
 * @parameter: 
 *      filp : 要打开的设备文件(文件描述符)
 *      cnt : 要读取的数据长度
 *      offt : 相对于文件首地址的偏移
 * @return: 
 *     success: 读取的字节数
 *     error: 负值
 * @note: 
 */
static ssize_t led_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    return 0;
}

/**
 * @function: 向设备写数据
 * @parameter: 
 *       filp : 设备文件，表示打开的文件描述符
 *       buf : 要写给设备写入的数据
 *       cnt : 要写入的数据长度
 *       offt : 相对于文件首地址的偏移
 * @return: 
 *     success: 写入的字节数
 *     error: 负值
 * @note: 
 */
static ssize_t led_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
    int retvalue;
    unsigned char databuf[1];
    unsigned char ledstat;
    struct gpioled_dev *dev = filp->private_data;

    retvalue = copy_from_user(databuf, buf, cnt);
    if(retvalue < 0)
    {
        printk("kernel write failed!\r\n");
        return -EFAULT;
    }

    ledstat = databuf[0]; /* 获取状态值 */

    if(ledstat == LEDON)
    {
        gpio_set_value(dev->led_gpio, 0); /* 打开 LED 灯 */
    }
    else if(leadstat == LEDOFF)
    {
        gpio_set_value(dev->led_gpio, 1); /* 关闭 LED 灯 */
    }

    return 0;
}

/**
 * @function: 关闭/释放设备
 * @parameter: 
 *      filp : 要关闭的设备文件(文件描述符)
 * @return: 
 *     success: 0
 *     error: 其他
 * @note: 
 */
static int led_release(struct inode *inode, struct file *filp)
{
    struct gpioled_dev *dev = filp->private_data;

    /* 关闭驱动文件的时候释放原子变量 */
    /* 每次退出就加一 */
    atomic_inc(&dev->lock);

    return 0;
}

/* 设备操作函数 */
static struct file_operations gpioled_fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .read = led_read,
    .write = led_write,
    .release = led_release,
};

/**
 * @function: 驱动入口函数
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static int __init led_init(void)
{
    int ret = 0;

    /* 初始化原子变量 */
    atomic_set(&gpioled.lock, 1); /* 原子变量初始值为 1 */
    /* 设置 LED 所使用的 GPIO */
    /* 1、获取设备节点： gpioled */
    gpioled.nd = of_find_node_by_path("/gpioled");
    if(gpioled.nd == NULL) 
    {
        printk("gpioled node not find!\r\n");
        return -EINVAL;
    }
    else
    {
        printk("gpioled node find!\r\n");
    }

    /* 2、 获取设备树中的gpio属性，得到LED所使用的LED编号 */
    gpioled.led_gpio = of_get_named_gpio(gpioled.nd, "led-gpio", 0);
    if(gpioled.led_gpio < 0)
    {
        printk("can't get led-gpio");
        return -EINVAL;
    }
    printk("led-gpio num = %d\r\n", gpioled.led_gpio);

    /* 3、设置GPIO1_IO03为输出，并且输出高电平，默认关闭LED灯 */
    ret = gpio_direction_output(gpioled.led_gpio, 1);
    if(ret < 0)
    {
        printk("can't set gpio!\r\n");
    }

    /* 注册字符设备驱动 */
	/* 1、创建设备号 */
    if(gpioled.major) 
    {
        /*  定义了设备号 */
        gpioled.devid = MKDEV(gpioled.major, 0);
        /* 注册设备 */
        register_chrdev_region(gpioled.devid, GPIOLED_CNT, GPIOLED_NAME);
    }
    else
    {
        /* 没有定义设备号 */
        alloc_chrdev_region(&gpioled.devid, 0, GPIOLED_CNT, GPIOLED_NAME);  /* 申请设备号 */
        /* 获取分配号的主设备号 */
        gpioled.major = MAJOR(gpioled.devid);

        /* 获取分配号的次设备号 */
        gpioled.minor = MINOR(gipioled.devid);
    }
    printk("gpioled major = %d, minor = %d\r\n", gpioled.major, gpioled.minor);

    /* 2、初始化cdev */
    gpioled.cdev.owner = THIS_MODULE;
    cdev_init(&gpioled.cdev, &gpioled_fops);

    /* 3、添加一个cdev */
    cdev_add(&gpioled.cdev, gpioled.devid, GPIOLED_CNT);


    /* 4、创建类 */
    gpioled.class = class_create(THIS_MODULE, GPIOLED_NAME);
    if(IS_ERR(gpioled.class)) 
    {
        return PTR_ERR(gpioled.class);
    }
    
    /* 5、创建设备 */
    gpioled.device = device_create(gpioled.class, NULL, gpioled.devid, NULL, GPIOLED_NAME);
    if(IS_ERR(gpioled.device))
    {
        return PTR_ERR(gpioled.device);
    }

    return 0;
}

/**
 * @function: 驱动出口函数
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static void __exit led_exit(void)
{
    /* 注销字符设备驱动 */
    cdev_del(&gpioled.cdev);/*  删除cdev */

    unregister_chrdev_region(gpioled.devid, GPIOLED_CNT); /* 注销设备号 */
    /* 注销设备 */
    device_destroy(gpioled.class, gpioled.devid);
    /* 注销类 */
    class_destroy(gpioled.class);
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("cpucode");


