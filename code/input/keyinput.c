/*
 * @Author: cpu_code
 * @Date: 2020-06-05 09:52:27
 * @LastEditTime: 2020-06-05 19:59:47
 * @FilePath: \Linux_driver\code\input\keyinput.c
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
#include <linux/input.h>
#include <linux/semaphore.h>
#include <linux/timer.h>
#include <linux/of_irq.h>
#include <linux/irq.h>

#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#define KEYINPUT_CNT		1			/* 设备号个数 	*/
#define KEYINPUT_NAME		"keyinput"	/* 名字 		*/
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

/* keyinput设备结构体 */
struct keyinput_dev
{
    dev_t devid;			/* 设备号 	 */
    struct cdev cdev;		/* cdev 	*/
    struct class *class;	/* 类 		*/
    struct device *device;	/* 设备 	 */
    struct device_node	*nd; /* 设备节点 */
    struct timer_list timer;/* 定义一个定时器*/
    struct irq_keydesc irqkeydesc[KEY_NUM];	/* 按键描述数组 */
    unsigned char curkeynum;				/* 当前的按键号 */
    struct input_dev *inputdev;		/* input结构体 */
};

struct keyinput_dev keyinputdev;	/* key input设备 */

/**
 * @function: 中断服务函数，开启定时器，延时10ms，定时器用于按键消抖
 * @parameter: 
 *      irq 	: 中断号
 *      dev_id	: 设备结构
 * @return: 
 *     success:  中断执行结果
 *     error:
 * @note: 
 */
static irqreturn_t key0_handler(int irq, void *dev_id)
{
    struct keyinput_dev *dev = (struct keyinput_dev *)dev_id;

    dev->curkeynum = 0;
	dev->timer.data = (volatile long)dev_id;

    mod_timer(&dev->timer, jiffies + msecs_to_jiffies(10));	/* 10ms定时 */

    return IRQ_RETVAL(IRQ_HANDLED);
}

/**
 * @function: 定时器服务函数，用于按键消抖，定时器到了以后
 *				  再次读取按键值，如果按键还是处于按下状态就表示按键有效。
 * @parameter: 
 *      arg	: 设备结构变量
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
	struct keyinput_dev *dev = (struct keyinput_dev *)arg;

    num = dev->curkeynum;
	keydesc = &dev->irqkeydesc[num];

    value = gpio_get_value(keydesc->gpio); 	/* 读取IO值 */
    if(value == 0)
    {
        /* 按下按键 */
		/* 上报按键值 */
        //input_event(dev->inputdev, EV_KEY, keydesc->value, 1);
        /* 最后一个参数表示按下还是松开，1为按下，0为松开 */
		input_report_key(dev->inputdev, keydesc->value, 1); 
        /* 同步事件 */
        input_sync(dev->inputdev);  
    }
    else
    {
        /* 按键松开 */
        //input_event(dev->inputdev, EV_KEY, keydesc->value, 0);
		input_report_key(dev->inputdev, keydesc->value, 0);
        /* 同步事件 */
        input_sync(dev->inputdev);
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

    //查找节点
    keyinputdev.nd = of_find_node_by_path("/key");
    if(keyinputdev.nd == NULL)
    {
        printk("key node not find!\r\n");
		return -EINVAL;
    }

    /* 提取GPIO */
	for(i = 0; i < KEY_NUM; i++)
    {
        //获取 GPIO
        keyinputdev.irqkeydesc[i].gpio = of_get_named_gpio(keyinputdev.nd ,"key-gpio", i);
        if(keyinputdev.irqkeydesc[i].gpio < 0)
        {
            printk("can't get key%d\r\n", i);
        }
    }

    /* 初始化key所使用的IO，并且设置成中断模式 */
	for(i = 0; i < KEY_NUM; i++)
    {
        memset(keyinputdev.irqkeydesc[i].name, 0, sizeof(name));	/* 缓冲区清零 */
        sprintf(keyinputdev.irqkeydesc[i].name, "KEY%d", i);		/* 组合名字 */
        //申请 GPIO
        gpio_request(keyinputdev.irqkeydesc[i].gpio, name);
        //设置 GPIO 为输入
        gpio_direction_input(keyinputdev.irqkeydesc[i].gpio);
        //提取中断号
        keyinputdev.irqkeydesc[i].irqnum = irq_of_parse_and_map(keyinputdev.nd, i);
    }

	keyinputdev.irqkeydesc[0].handler = key0_handler;
    keyinputdev.irqkeydesc[0].value = KEY_0;

    for(i = 0; i < KEY_NUM; i++)
    {
        //申请中断
        ret = request_irq(keyinputdev.irqkeydesc[i].irqnum, 
                          keyinputdev.irqkeydesc[i].handler, 
		                  IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, 
                          keyinputdev.irqkeydesc[i].name, 
                          &keyinputdev);
        if(ret < 0)
        {
            printk("irq %d request failed!\r\n", keyinputdev.irqkeydesc[i].irqnum);
			return -EFAULT;
        }
    }

    /* 创建定时器 */
	init_timer(&keyinputdev.timer);
    keyinputdev.timer.function = timer_function;

    /* 申请input_dev */
	keyinputdev.inputdev = input_allocate_device();
    /* 设置 input_dev 名字 */
	keyinputdev.inputdev->name = KEYINPUT_NAME;

#if 0
    /* 初始化input_dev，设置产生哪些事件 */
	__set_bit(EV_KEY, keyinputdev.inputdev->evbit);	/* 设置产生按键事件 */

    /* 重复事件，比如按下去不放开，就会一直输出信息 */
    __set_bit(EV_REP, keyinputdev.inputdev->evbit);

    /* 初始化input_dev，设置产生哪些按键 */
	__set_bit(KEY_0, keyinputdev.inputdev->keybit);	
#endif

#if 0
    keyinputdev.inputdev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REP);
    keyinputdev.inputdev->keybit[BIT_WORD(KEY_0)] |= BIT_MASK(KEY_0);
#endif
    keyinputdev.inputdev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REP);
    /* 设置 EV_KEY 事件以及 KEY 的按键类型 */
    input_set_capability(keyinputdev.inputdev, EV_KEY, KEY_0);

    /* 注册输入设备 */
	ret = input_register_device(keyinputdev.inputdev);
    if(ret)
    {
        printk("register input device failed!\r\n");
		return ret;
    }
    
	return 0;
}

/**
 * @function: 驱动入口函数
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static int __init keyinput_init(void)
{
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
static void __exit keyinput_exit(void)
{
    unsigned int i = 0;

    /* 删除定时器 */
	del_timer_sync(&keyinputdev.timer);

    /* 释放中断 */
	for (i = 0; i < KEY_NUM; i++) 
    {
        free_irq(keyinputdev.irqkeydesc[i].irqnum, &keyinputdev);
    }

    /* 释放input_dev */
	input_unregister_device(keyinputdev.inputdev);
    /* 删除 input_dev */
	input_free_device(keyinputdev.inputdev);
}

module_init(keyinput_init);
module_exit(keyinput_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("cpucode");