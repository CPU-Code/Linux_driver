/**
 * @Author: cpu_code
 * @Date: 2020-05-17 18:05:14
 * @LastEditTime: 2020-05-17 18:12:52
 * @FilePath: \Linux_driver\3newchrled\newchrled.c
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

#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#define NEWCHRLED_CNT			1		  	/* 设备号个数 */
#define NEWCHRLED_NAME			"newchrled"	/* 名字 */
#define LEDOFF 					0			/* 关灯 */
#define LEDON 					1			/* 开灯 */

/* 寄存器物理地址 */
#define CCM_CCGR1_BASE				(0X020C406C)	
#define SW_MUX_GPIO1_IO03_BASE		(0X020E0068)
#define SW_PAD_GPIO1_IO03_BASE		(0X020E02F4)
#define GPIO1_DR_BASE				(0X0209C000)
#define GPIO1_GDIR_BASE				(0X0209C004)

/* 映射后的寄存器虚拟地址指针 */
static void __iomem *IMX6U_CCM_CCGR1;
static void __iomem *SW_MUX_GPIO1_IO03;
static void __iomem *SW_PAD_GPIO1_IO03;
static void __iomem *GPIO1_DR;
static void __iomem *GPIO1_GDIR;

struct newchrled_dev
{
    dev_t devid;			/* 设备号 	  */
    /** 
     *cdev 结构体表示一个字符设备
     * cdev 结构体在 include/linux/cdev.h
     */
    struct cdev cdev;		/* cdev   	  */
    struct class *class;	/* 类 		  */
    struct device *device;	/* 设备 	  */
    int major;				/* 主设备号	  */
    int minor;				/* 次设备号   */
};

struct newchrled_dev newchrled;	 /* led设备 */

/* 设备操作函数 */
static struct file_operations newchrled_fops = {
	.owner = THIS_MODULE,
	.open = led_open,
	.read = led_read,
	.write = led_write,
	.release = 	led_release,
};

/**
 * @function: LED打开/关闭
 * @parameter: 
 *      sta: LEDON(0) 打开LED
 *           LEDOFF(1) 关闭LED
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
void led_switch(u8 sta)
{
    u32 val = 0;

    if(sta == LEDON)
    {
        val = readl(GPIO1_DR);
        val &= ~(1 << 3);
        writel(val, GPIO1_DR);
    }
    else if(sta == LEDOFF)
    {
        val = readl(GPIO1_DR);
        val |= (1 << 3);
        writel(val, GPIO1_DR);
    }
}

/**
 * @function: 打开设备
 * @parameter: 
 *      inode: 传递给驱动的inode
 *       filp: 设备文件，file结构体有个叫做private_data的成员变量
 *             一般在open的时候将private_data指向设备结构体
 * @return: 
 *     success: 0
 *     error: 其他
 * @note: 
 */
static int led_open(struct inode *inode, struct file *filp)
{
    filp->private_data = &newchrled; /* 设置私有数据 */

    return 0;
}

/**
 * @function: 从设备读取数据
 * @parameter: 
 *      filp : 要打开的设备文件(文件描述符)
 *      buf : 返回给用户空间的数据缓冲区
 *      cnt : 要读取的数据长度
 *      offt  相对于文件首地址的偏移
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
 * @function: 
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static ssize_t led_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
    int retvalue;
}

/**
 * @function: 关闭/释放设备
 *      filp:要关闭的设备文件(文件描述符)
 * @parameter: 
 * @return: 
 *     success: 0
 *     error:其他
 * @note: 
 */
static int led_release(struct inode *inode, struct file *filp)
{
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
static int __init led_init(void)
{
    u32 val = 0;

    /* 初始化LED */
	/* 1、寄存器地址映射 */
    IMX6U_CCM_CCGR1 = ioremap(CCM_CCGR1_BASE, 4);
	SW_MUX_GPIO1_IO03 = ioremap(SW_MUX_GPIO1_IO03_BASE, 4);
  	SW_PAD_GPIO1_IO03 = ioremap(SW_PAD_GPIO1_IO03_BASE, 4);
	GPIO1_DR = ioremap(GPIO1_DR_BASE, 4);
	GPIO1_GDIR = ioremap(GPIO1_GDIR_BASE, 4);

    /* 2、使能GPIO1时钟 */
    val = readl(IMX6U_CCM_CCGR1);
    val &= ~(3 << 26);	/* 清楚以前的设置 */
    val |= (3 << 26);	/* 设置新值 */
    writel(val, IMX6U_CCM_CCGR1);

    /**
     *3、设置GPIO1_IO03的复用功能，将其复用为
	 *    GPIO1_IO03，最后设置IO属性。
	 */
    writel(5, SW_MUX_GPIO1_IO03);

    /**
     *寄存器SW_PAD_GPIO1_IO03设置IO属性
	 *bit 16:0 HYS关闭
	 *bit [15:14]: 00 默认下拉
     *bit [13]: 0 kepper功能
     *bit [12]: 1 pull/keeper使能
     *bit [11]: 0 关闭开路输出
     *bit [7:6]: 10 速度100Mhz
     *bit [5:3]: 110 R0/6驱动能力
     *bit [0]: 0 低转换率
	 */
    writel(0x10B0, SW_PAD_GPIO1_IO03);

    /* 4、设置GPIO1_IO03为输出功能 */
    val = readl(GPIO1_GDIR);
    val &= ~(1 << 3);	/* 清除以前的设置 */
	val |= (1 << 3);	/* 设置为输出 */
    writel(val, GPIO1_GDIR);

    /* 5、默认关闭LED */
    val = readl(GPIO1_DR);
    val |= (1 << 3);
    writel(val, GPIO1_DR);

    /* 注册字符设备驱动 */
	/* 1、创建设备号 */
    if(newchrled.major)
    {
        /*  定义了设备号 */

        //将给定的主设备号和次设备号的值组合成 dev_t 类型的设备号
        newchrled.devid = MKDEV(newchrled.major, 0);

        /**
         * @function: 注册设备号
         * @parameter: 
         *      newchrled.devid：要申请的起始设备号，也就是给定的设备号
         *      NEWCHRLED_CNT： 要申请的数量
         *      NEWCHRLED_NAME：设备名字
         * @return: 
         *     success: 
         *     error:
         * @note: 
         */
        register_chrdev_region(newchrled.devid, NEWCHRLED_CNT, NEWCHRLED_NAME);
    }
    else
    {
        /* 没有定义设备号 */

        /**
         * @function:  申请设备号 
         * @parameter: 
         *      newchrled.devid：保存申请到的设备号
         *      0：次设备号起始地址
         *      NEWCHRLED_CNT：要申请的设备号数量
         *      NEWCHRLED_NAME：设备名字
         * @return: 
         *     success: 
         *     error:
         * @note: 
         */
        alloc_chrdev_region(&newchrled.devid, 0, NEWCHRLED_CNT, NEWCHRLED_NAME);
        newchrled.major = MAJOR(newchrled.devid);	/* 获取分配号的主设备号 */
        newchrled.minor = MINOR(newchrled.devid);	/* 获取分配号的次设备号 */
    }
    //在终端上显示出申请到的主设备号和次设备号
    printk("newcheled major=%d, minor=%d\r\n", newchrled.major, newchrled.minor);

    /* 2、初始化cdev */
	newchrled.cdev.owner = THIS_MODULE;
    cdev_init(&newchrled.cdev, &newchrled_fops);

    /* 3、添加一个cdev */
    cdev_add(&newchrled.cdev, newchrled.devid, NEWCHRLED_CNT);

    /* 4、创建类 */
    /* 文件include/linux/device.h  */
    newchrled.class = class_create(THIS_MODULE, NEWCHRLED_NAME);
    if(IS_ERR(newchrled.class))
    {
        return PTR_ERR(newchrled.class);
    }

    /* 5、创建设备 */
    newchrled.device = device_create(newchrled.class, NULL, newchrled.devid, NULL, NEWCHRLED_NAME);
    if(IS_ERR(newchrled.device))
    {
        return PTR_ERR(newchrled.device);
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
    /* 取消映射 */
    iounmap(IMX6U_CCM_CCGR1);
	iounmap(SW_MUX_GPIO1_IO03);
	iounmap(SW_PAD_GPIO1_IO03);
	iounmap(GPIO1_DR);
	iounmap(GPIO1_GDIR);

    /* 注销字符设备驱动 */
	cdev_del(&newchrled.cdev);/*  删除cdev */

    /**
     * @function: 注销设备号
     * @parameter: 
     *      newchrled.devid：要释放的设备号
     *      NEWCHRLED_CNT： NEWCHRLED_CNT
     * @return: 
     *     success: 
     *     error:
     * @note: 
     */
    unregister_chrdev_region(newchrled.devid, NEWCHRLED_CNT); 

    /* 设备删除 */
    device_destroy(newchrled.class, newchrled.devid);
    /**
     * @function: 删除掉类
     * @parameter: 
     *      newchrled.class:要删除的类
     * @return: 
     *     success: 
     *     error:
     * @note:  文件include/linux/device.h
     */
    class_destroy(newchrled.class);
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("cpucode");