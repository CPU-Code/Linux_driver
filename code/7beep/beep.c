/*
 * @Author: cpu_code
 * @Date: 2020-05-23 22:54:07
 * @LastEditTime: 2020-05-24 11:56:47
 * @FilePath: \Linux_driver\7beep\beep.c
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

#define BEEP_CNT    1       /* 设备号个数 */
#define BEEP_NAME   "beep0" /* 名字 */
#define BEEPOFF     0       /* 关蜂鸣器 */
#define BEEPON      1       /* 开蜂鸣器 */

/* beep 设备结构体 */

struct beep_dev
{
    dev_t           devid;  /* 设备号 */
    struct  cdev    cdev;   /* cdev */
    struct  class   *class; /* 类 */
    int     device  *device /* 设备 */
    int             major;  /* 主设备号 */
    int             minor;  /* 次设备号 */
    struct device_node *nd; /* 设备节点 */
    int             beep_gpio;  /* beep使用的GPIO编码 */
};

struct beep_dev     beep;       /* beep设备 */


/**
 * @function: 打开设备
 * @parameter: 
 *      inode : 传递给驱动的 inode
 *       filp : 设备文件， file 结构体有个叫做 private_data 的成员变量，
 *              一般在 open 的时候将 private_data 指向设备结构体
 * @return: 
 *     success: 0
 *     error: 失败
 * @note: 
 */
static int beep_open(struct inode *inode, struct file *filp)
{
    filp->private_data = &beep;     /* 设置私有数据 */
    return 0;
}

/**
 * @function: 向设备写数据
 * @parameter: 
 *      filp : 设备文件，表示打开的文件描述符
 *      buf : 要写给设备写入的数据
 *      cnt : 要写入的数据长度
 *      offt : 相对于文件首地址的偏移
 * @return: 
 *     success: 写入的字节数
 *     error: 负值
 * @note: 
 */
static ssize_t beep_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
    int             retvalue = 0;
    unsigned char   databuf[1] = {0};
    unsigned char   beepstat = 0;
    struct beep_dev *dev = filp->private_data;
    
    retvalue = copy_from_user(databuf, buf, cnt);
    if(retvalue < 0)
    {
        printfk("kernel write failed!\r\n");
        return -EFAULT;
    }

    beepstat = databuf[0];      /* 获取状态 */

    if(beepstat == BEEPON)
    {
        /* 打开蜂鸣器 */
        gpio_set_value(dev->beep_gpio, 0);
    }
    else if(beepstat == BEEPOFF)
    {
        /* 关闭蜂鸣器 */
        gpio_set_value(dev->beep_gpio, 1);
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
static int beep_release(struct inode *inode, struct filp *filp)
{
    return 0;
}

/* 设备操作函数 */
static struct file_operations beep_fops = {
    .owner = THIS_MODULE;
    .open  = beep_open;
    .write = beep_write;
    .release = beep_release;
};

/**
 * @function: 驱动入口函数
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static int __init beep_init()
{
    int ret = 0;

    /* 设置BEEP使用的GPIO */
    /* 获取设备节点：beep */
    beep.nd = of_find_node_by_path("/beep");
    if(beep.nd == NULL)
    {
        printk("beep node not find!\r\n");
        return -EINVAL;
    }
    else
    {
        printk("beep node find!\r\n");
    }

    /* 获取设备树中的 gpio 属性，得到 BEEP 所使用的 GPIO 编号  */
    beep.beep_gpio = of_get_named_gpio(beep.nd, "beep-gpio", 0);
    if(beep.beep_gpio < 0)
    {
        printk("can't get beep-gpio!");
        return -EINVAL;
    }
    printk("led-gpio num = %d\r\n", beep.beep_gpio);

    /* 设置GPIO5_IO1为输出， 并输出高电平，默认关闭BEEP */
    ret = gpio_direction_output(beep.beep_gpio, 1);
    if(ret < 0)
    {
        printk("cant't set gpio!\r\n");
    }

    /* 注册字符设备驱动 */
    /* 创建设备号 */
    if(beep.major)
    {
        /* 定义设备号 */
        beep.devid = MKDEV(beep.major, 0);
        /* 注册设备号 */
        register_chrdev_region(beep.devid, BEEP_CNT, BEEP_NAME);
    }
    else
    {
        /* 没有定义设备号 */
        alloc_chrdev_region(&beep.devid, 0, BEEP_CNT, BEEP_NAME);
         /* 获取分配号的主设备号 */
        bepp.major = MAJOR(beep.devid);
        /* 获取分配号的次设备号 */
        beep.minor = MINOR(beep.devid); 
    }
    
    printk("beep major = %d,minor = %d\r\n", beep.major, beep.minor);

    /* 2、初始化 cdev */
    beep.cdev.owner = THIS_MODULE;
    cdev_init(&beep.cdev, &beep_fops);

    /* 3、添加一个 cdev */
    cdev_add(&beep.cdev, beep.devid, BEEP_CNT);

    /* 4、创建类 */
    beep.class = class_create(THIS_MODULE, BEEP_NAME);
    if(IS_ERR(beep.class))
    {
        return PTR_ERR(beep.class);
    }

    /* 5、创建设备 */
    beep.device = device_create(beep.class, NULL, beep.devid, NULL, BEEP_NAME);
    if (IS_ERR(beep.device)) 
    {
        return PTR_ERR(beep.device);
    }

    return 0;
}

/**
 * @function:  驱动出口函数
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static void __exit beep_exit(void)
{
    /* 注销字符设备驱动 */
    cdev_del(&beep.cdev); /* 删除 cdev */
    unregister_chrdev_region(beep.devid, BEEP_CNT); /* 注销设备号 */
    /* 删除创建的设备 */
    device_destroy(beep.class, beep.devid);
    /* 删除类 */
    class_destroy(beep.class);
}

module_init(beep_inti);
module_exit(beep_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("cpucode");
