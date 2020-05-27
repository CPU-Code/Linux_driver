/*
 * @Author: cpu_code
 * @Date: 2020-05-27 14:12:32
 * @LastEditTime: 2020-05-27 14:38:10
 * @FilePath: \Linux_driver\code\14irq\irq.c
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
#include <linux/semaphore.h>
#include <linux/timer.h>
#include <linux/of_irq.h>
#include <linux/irq.h>

#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#define IMX6UIRQ_CNT		1			/* 设备号个数 	*/
#define IMX6UIRQ_NAME		"irq"	    /* 名字 		*/
#define KEY0VALUE			0X01		/* KEY0按键值 	*/
#define INVAKEY				0XFF		/* 无效的按键值 */
#define KEY_NUM				1			/* 按键数量 	*/

/* 中断IO描述结构体 */
struct irq_keydesc
{
    int gpio;								/* gpio */
    int irqnum;								/* 中断号     */
    unsigned char value;					/* 按键对应的键值 */
    char name[10];							/* 名字 */
    irqreturn_t (*handler)(int, void *);	/* 中断服务函数 */
};

/* imx6uirq设备结构体 */
struct irq_dev
{
    dev_t devid;			/* 设备号 	 */
    struct cdev cdev;		/* cdev 	*/
    struct class *class;	/* 类 		*/
    struct device *device;	/* 设备 	 */
    int major;				/* 主设备号	  */
    int minor;				/* 次设备号   */
    struct device_node	*nd; /* 设备节点 */
    atomic_t keyvalue;		/* 有效的按键键值 */
    atomic_t releasekey;	/* 标记是否完成一次完成的按键，包括按下和释放 */
    struct timer_list timer;/* 定义一个定时器*/
    struct irq_keydesc irqkeydesc[KEY_NUM];	/* 按键描述数组 */
    unsigned char curkeynum;				/* 当前的按键号 */
};

struct irq_dev irq;	/* irq设备 */

/**
 * @function: 中断服务函数，开启定时器，延时10ms，定时器用于按键消抖
 * @parameter: 
 *      irq 	: 中断号
 *      dev_id	: 设备结构
 * @return: 
 *     success: 中断执行结果
 *     error:
 * @note: 
 */
static irqreturn_t key0_handler(int irq, void *dev_id)
{
    struct irq_dev *dev = (struct irq_dev *)dev_id;

    dev->curkeynum = 0;
    dev->timer.data = (volatile long)dev_id;

    mod_timer(&dev->timer, jiffies + msecs_to_jiffies(10));	/* 10ms定时 */

    return IRQ_RETVAL(IRQ_HANDLED);
}

/**
 * @function:  定时器服务函数，用于按键消抖，定时器到了以后再次读取按键值，如 按键还是处于按下状态就表示按键有效
 * @parameter: 
 *       arg	: 设备结构变量
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
void timer_function(unsigned long arg)
{
    unsigned char value;
	unsigned char num;
	struct irq_keydesc *keydesc;

    struct irq_dev *dev = (struct irq_dev *)arg;

    num = dev->curkeynum;
    keydesc = &dev->irqkeydesc[num];

    value = gpio_get_value(keydesc->gpio); 	/* 读取IO值 */
    if(value == 0)
    {
        /* 按下按键 */
        atomic_set(&dev->keyvalue, keydesc->value);
    }
    else
    {
        /* 按键松开 */
        atomic_set(&dev->keyvalue, 0x80 | keydesc->value);

        atomic_set(&dev->releasekey, 1);	/* 标记松开按键，即完成一次完整的按键过程 */
    }
}

/**
 * @function: 按键IO初始化
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static int keyio_init(void)
{
    unsigned char i = 0;
	char name[10];
	int ret = 0;

    irq.nd = of_find_node_by_path("/key");
    if(imx6uirq.nd == NULL)
    {
        printk("key node not find!\r\n");
		return -EINVAL;
    }

    /* 提取GPIO */
	for(i = 0; i < KEY_NUM; i++)
    {
        irq.irqkeydesc[i].gpio = of_get_named_gpio(irq.nd ,"key-gpio", i);

        if(imx6uirq.irqkeydesc[i].gpio < 0)
        {
            printk("can't get key%d\r\n", i);
        }
    }

    /* 初始化key所使用的IO，并且设置成中断模式 */
	for(i = 0; i < KEY_NUM; i++)
    {

    }


    return 0;
}

/**
 * @function: 
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static int irq_open(struct inode *inode, struct file *filp)
{
    
}

/**
 * @function: 
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static ssize_t irq_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    
}


/* 设备操作函数 */
static struct file_operations irq_fops = {
	.owner = THIS_MODULE,
	.open = irq_open,
	.read = irq_read,
};

/**
 * @function: 驱动入口函数
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static int __init irq_init(void)
{

}

/**
 * @function: 驱动出口函数
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static void __exit irq_exit(void)
{

    
}

module_init(irq_init);
module_exit(irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("cpucode");