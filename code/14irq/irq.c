/*
 * @Author: cpu_code
 * @Date: 2020-05-27 14:12:32
 * @LastEditTime: 2020-05-27 20:06:42
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

#define IRQ_CNT		        1			/* 设备号个数 	*/
#define IRQ_NAME		    "irq"	    /* 名字 		*/
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

/* irq设备结构体 */
struct irq_dev
{
    dev_t               devid;			/* 设备号 	 */
    struct cdev         cdev;		    /* cdev 	*/
    struct class        *class;	        /* 类 		*/
    struct device       *device;	    /* 设备 	 */
    int                 major;			/* 主设备号	  */
    int                 minor;			/* 次设备号   */
    struct device_node	*nd;            /* 设备节点     */
    atomic_t           keyvalue;		/* 有效的按键键值 */
    atomic_t           releasekey;	    /* 标记是否完成一次完成的按键，包括按下和释放 */
    struct timer_list  timer;           /* 定义一个定时器   */
    struct irq_keydesc irqkeydesc[KEY_NUM];	/* 按键描述数组 */
    unsigned char      curkeynum;			/* 当前的按键号 */
};

struct irq_dev irq; 	/* irq设备 */

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
    if(irq.nd == NULL)
    {
        printk("key node not find!\r\n");
		return -EINVAL;
    }

    /* 提取GPIO */
	for(i = 0; i < KEY_NUM; i++)
    {
        irq.irqkeydesc[i].gpio = of_get_named_gpio(irq.nd ,"key-gpio", i);

        if(irq.irqkeydesc[i].gpio < 0)
        {
            printk("can't get key%d\r\n", i);
        }
    }

    /* 初始化key所使用的IO，并且设置成中断模式 */
	for(i = 0; i < KEY_NUM; i++)
    {
        memset(irq.irqkeydesc[i].name, 0, sizeof(name));	/* 缓冲区清零 */
        sprintf(irq.irqkeydesc[i].name, "KEY%d", i);		/* 组合名字 */

        gpio_request(irq.irqkeydesc[i].gpio, name);
        gpio_direction_input(irq.irqkeydesc[i].gpio);

        irq.irqkeydesc[i].irqnum = irq_of_parse_and_map(irq.nd, i);
#if 0
        irq.irqkeydesc[i].irqnum = gpio_to_irq(irq.irqkeydesc[i].gpio);
#endif
        printk("key%d:gpio = %d, irqnum = %d\r\n", i, 
                                                   irq.irqkeydesc[i].gpio, 
                                                   irq.irqkeydesc[i].irqnum);
    }
    /* 申请中断 */
    irq.irqkeydesc[0].handler = key0_handler;
    irq.irqkeydesc[0].value = KEY0VALUE;

    for(i = 0; i < KEY_NUM; i++)
    {
        ret = request_irq(irq.irqkeydesc[i].irqnum, 
                          irq.irqkeydesc[i].handler, 
		                  IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, 
                          irq.irqkeydesc[i].name, 
                          &irq);
        if(ret < 0)
        {
            printk("irq %d request failed!\r\n", irq.irqkeydesc[i].irqnum);
            return -EFAULT;
        }
    }

    /* 创建定时器 */
    init_timer(&irq.timer);
    irq.timer.function = timer_function;

    return 0;
}

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
static int irq_open(struct inode *inode, struct file *filp)
{
    filp->private_data = &irq;	/* 设置私有数据 */

    return 0;
}

/**
 * @function: 从设备读取数据
 * @parameter: 
 *      filp    : 要打开的设备文件(文件描述符)
 *      buf     : 返回给用户空间的数据缓冲区
 *      cnt     : 要读取的数据长度
 *      offt    : 相对于文件首地址的偏移
 * @return: 
 *     success: 读取的字节数
 *     error: 负值
 * @note: 
 */
static ssize_t irq_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    int ret = 0;
	unsigned char keyvalue = 0;
	unsigned char releasekey = 0;

    struct irq_dev *dev = (struct irq_dev *)filp->private_data;

    keyvalue = atomic_read(&dev->keyvalue);
    releasekey = atomic_read(&dev->releasekey);

    if(releasekey)
    {
        /* 有按键按下 */
        if(keyvalue & 0x80) 
        {
            keyvalue &= ~0x80;
            ret = copy_to_user(buf, &keyvalue, sizeof(keyvalue));
        }
        else
        {
            goto data_error;
        }
        atomic_set(&dev->releasekey, 0);/* 按下标志清零 */
    }
    else
    {
        goto data_error;
    }

    return 0;
    
data_error:
	return -EINVAL;
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
    /* 1、构建设备号 */
	if(irq.major)
    {
        irq.devid = MKDEV(irq.major, 0);
        register_chrdev_region(irq.devid, IRQ_CNT, IRQ_NAME);
    }
    else
    {
        alloc_chrdev_region(&irq.devid, 0, IRQ_CNT, IRQ_NAME);
        irq.major = MAJOR(irq.devid);
        irq.minor = MINOR(irq.devid);
    }

    /* 2、注册字符设备 */
    cdev_init(&irq.cdev, &irq_fops);
    cdev_add(&irq.cdev, irq.devid, IRQ_CNT);

    /* 3、创建类 */
    irq.class = class_create(THIS_MODULE, IRQ_NAME);
    if(IS_ERR(irq.class))
    {
        return PTR_ERR(irq.class);
    }

    /* 4、创建设备 */
    irq.device = device_create(irq.class, NULL, irq.devid, NULL, IRQ_NAME);
    if(IS_ERR(irq.device))
    {
        return PTR_ERR(irq.device);
    }

    /* 5、初始化按键 */
    atomic_set(&irq.keyvalue, INVAKEY);
    atomic_set(&irq.releasekey, 0);

    keyio_init();

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
static void __exit irq_exit(void)
{
    unsigned int i = 0;
    
    /* 删除定时器 */
	del_timer_sync(&irq.timer);

    /* 释放中断 */
	for(i = 0; i < KEY_NUM; i++)
    {
        free_irq(irq.irqkeydesc[i].irqnum, &irq);
    }
    cdev_del(&irq.cdev);

    unregister_chrdev_region(irq.devid, IRQ_CNT);

    device_destroy(irq.class, irq.devid);

    class_destroy(  irq.class);
}

module_init(irq_init);
module_exit(irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("cpucode");