/*
 * @Author: cpu_code
 * @Date: 2020-05-17 14:15:37
 * @LastEditTime: 2020-05-24 11:30:55
 * @FilePath: \Linux_driver\2Led\led.c
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

#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#define LED_MAJOR		200		/* 主设备号 */
#define LED_NAME		"led" 	/* 设备名字 */

#define LEDOFF 	0				/* 关灯 */
#define LEDON 	1				/* 开灯 */

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

/**
 * @function:  LED打开/关闭
 * @parameter: 
 * 		sta: LEDON(0) 打开LED
 * 	 		 LEDOFF(1) 关闭LED
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
		/**
		 * @function:  读操作
		 * @parameter: 
		 * 		GPIO1_DR：要读取写内存地址
		 * @return: 
		 *     success: 读取到的数据
		 *     error:
		 * @note: readb，readw， readl 这三个函数分别对应 8bit、16bit 、32bit 读操作
		 */
		val = readl(GPIO1_DR);
		val &= ~(1 << 3);
		
		/**
		 * @function: 写操作
		 * @parameter: 
		 * 		val：要写入的数值
		 * 		addr：要写入的地址
		 * @return: 
		 *     success: 
		 *     error:
		 * @note: writeb、writew、writel 这三个函数分别对应 8bit、16bit、32bit 写操作
		 */
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
 * 		inode：传递给驱动的inode
 * 		filp：设备文件，file结构体有个叫做private_data的成员变量
 * 			  一般在open的时候将private_data指向设备结构体
 * @return: 
 *     success: 0
 *     error:其他
 * @note: 其他
 */
static int led_open(struct inode *inode, struct file *filp)
{
	printk("led_open !\r\n");
	return 0;
}

/**
 * @function: 从设备读取数据
 * @parameter: 
 * 		filp: 要打开的设备文件(文件描述符)
 * 		buf: 返回给用户空间的数据缓冲区
 * 		cnt: 要读取的数据长度
 * 		offt: 相对于文件首地址的偏移
 * @return: 
 *     success: 读取的字节数
 *     error: 负值
 * @note: 
 */
static ssize_t led_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
	printk("led_read !\r\n");
	return 0;
}

/**
 * @function: 向设备写数据 
 * @parameter: 
 * 		filp: 设备文件，表示打开的文件描述符
 * 		buf: 要写给设备写入的数据
 * 		cnt：要写入的数据长度
 * 		offt：相对于文件首地址的偏移
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

	retvalue = copy_from_user(databuf, buf, cnt);
	if(retvalue < 0)
	{
		printk("kernel write failed!\r\n");
		return -EFAULT;
	}

	ledstat = databuf[0];		/* 获取状态值 */
	if(ledstat == LEDON)
	{
		led_switch(LEDON);		/* 打开LED灯 */
	}
	else if(ledstat == LEDOFF)
	{
		led_switch(LEDOFF);	/* 关闭LED灯 */
	}

	return 0;
}

/**
 * @function: 关闭/释放设备
 * @parameter: 
 * 		inode: 传递给驱动的inode
 * 		filp: 要关闭的设备文件(文件描述符)
 * @return: 
 *     success: 0
 *     error:其他
 * @note: 
 */
static int led_release(struct inode *inode, struct file *filp)
{
	printk("led_release\r\n");
	return 0;
}

/* 设备操作函数 */
static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.open = led_open,
	.read = led_read,
	.write = led_write,
	.release = 	led_release,
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
	int retvalue = 0;
	u32 val = 0;

	/* 初始化LED */
	/* 1、寄存器地址映射 */
	/**
	 * @function: 获取指定物理地址空间对应的虚拟地址空间
	 * @parameter: 
	 * 		CCM_CCGR1_BASE：要映射给的物理起始地址
	 * 		4：要映射的内存空间大小
	 * @return: 
	 *     success: 指向映射后的虚拟空间首地址
	 *     error:
	 * @note: 定义在 arch/arm/include/asm/io.h 文件
	 */
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

	/* 
	 * 3、设置GPIO1_IO03的复用功能，将其复用为
	 *    GPIO1_IO03，最后设置IO属性。
	 */
	writel(5, SW_MUX_GPIO1_IO03);

	/*寄存器SW_PAD_GPIO1_IO03设置IO属性
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

	/* 6、注册字符设备驱动 */
	retvalue = register_chrdev(LED_MAJOR, LED_NAME, &led_fops);
	if(retvalue < 0)
	{
		printk("register chrdev failed!\r\n");
		return -EIO;
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
	/**
     * @function: 释放掉 ioremap 函数所做的映射
     * @parameter: 
	 * 		IMX6U_CCM_CCGR1：要取消映射的虚拟地址空间首地址
     * @return: 
	 *     success: 
 	 *     error:
     * @note: 
     */
    iounmap(IMX6U_CCM_CCGR1);
	iounmap(SW_MUX_GPIO1_IO03);
	iounmap(SW_PAD_GPIO1_IO03);
	iounmap(GPIO1_DR);
	iounmap(GPIO1_GDIR);

    /* 注销字符设备驱动 */
	unregister_chrdev(LED_MAJOR, LED_NAME);
}


module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("cpucode");