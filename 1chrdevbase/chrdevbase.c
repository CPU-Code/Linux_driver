/*
 * @Author: cpu_code
 * @Date: 2020-05-16 21:43:02
 * @LastEditTime: 2020-05-17 14:14:58
 * @FilePath: \Linux_driver\1chrdevbase\chrdevbase.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */ 

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>

#define CHRDEVBASE_MAJOR	200				/* 主设备号 */
#define CHRDEVBASE_NAME		"chrdevbase" 	/* 设备名 */

static char readbuf[100];		/* 读缓冲区 */
static char writebuf[100];		/* 写缓冲区 */
static char kerneldata[] = {"kernel data!"};

/*
 * 设备操作函数结构体
 */
static struct file_operations chrdevbase_fops = {
	.owner = THIS_MODULE,
	.open = chrdevbase_open,
	.read = chrdevbase_read,
	.write = chrdevbase_write,
	.release = chrdevbase_release,
};

/**
 * @function: 打开设备
 * @parameter: 
 *      inode: 传递给驱动的inode
 *      filp: 设备文件，file结构体有个叫做private_data的成员变量
 * @return: 
 *     success: 0
 *     error:其他
 * @note: 
 */
static int chrdevbase_open(struct inode *inode, struct file *filp)
{
    /*  printf运行在用户态， printk 运行在内核态 */
    printk("chrdevbase open!\r\n");
    return 0;
}

/**
 * @function: 从设备读取数据
 * @parameter: 
 *      filp: 要打开的设备文件(文件描述符)
 *      buf: 返回给用户空间的数据缓冲区
 *      cnt: 要读取的数据长度
 *      offt: 相对于文件首地址的偏移
 * @return: 
 *     success: 读取的字节数
 *     error: 负值
 * @note: 
 */
static ssize_t chrdevbase_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt)
{
    int retvalue = 0;

    /* 向用户空间发送数据 */
    /* 将 kerneldata 数组中的数据拷贝到读缓冲区 readbuf */
    memcpy(readbuf, kerneldata, sizeof(kerneldata));

    /**
     * @function: 完成内核空间的数据到用户空间的复制
     * @parameter: 
     *       buf： 目的
     *       readbuf： 源  
     *       cnt： 要复制的数据长度
     * @return: 
    *     success: 0
    *     error:负值
     * @note: 
     */
    retvalue = copy_to_user(buf, readbuf, cnt);
    if(retvalue == 0)
    {
        printk("kernel senddata ok!\r\n");
    }
    else
    {
        printk("kernel senddata failed!\r\n");
    }
    
    printk("chrdevbase read!\r\n");
	return 0;
}

/**
 * @function: 向设备写数据 
 * @parameter: 
 *      filp: 设备文件，表示打开的文件描述符
 *      buf: 要写给设备写入的数据
 *      cnt: 要写入的数据长度
 *      offt: 相对于文件首地址的偏移
 * @return: 
 *     success:  写入的字节数
 *     error: 负值
 * @note: 
 */
static ssize_t chrdevbase_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt)
{
    int retvalue = 0;

    /* 接收用户空间传递给内核的数据并且打印出来 */
    retvalue = copy_from_user(writebuf, buf, cnt);
    if(retvalue == 0)
    {
        printk("kernel recevdata:%s\r\n", writebuf);
    }
    else
    {
        printk("kernel recevdata failed!\r\n");
    }

    printk("chrdevbase write!\r\n");
	return 0;
}

/**
 * @function: 关闭/释放设备
 * @parameter: 
 *       filp : 要关闭的设备文件(文件描述符)
 * @return: 
 *     success: 0
 *     error: 其他值
 * @note: 
 */
static int chrdevbase_release(struct inode *inode, struct file *filp)
{
    printk("chrdevbase release！\r\n");
	return 0;
}

/**
 * @function: 驱动入口函数 
 * @parameter: 
 * @return: 
 *     success: 0
 *     error: 其他
 * @note: 
 */
static int __init chrdevbase_init(void)
{
    int retvalue = 0;

    /**
     * @function: 注册字符设备驱动
     * @parameter: 
     *      CHRDEVBASE_MAJOR：主设备号
     *      CHRDEVBASE_NAME： 设备名字
     *      chrdevbase_fops：指向设备的操作函数集合变量
     * @return: 
     *     success: 
     *     error:
     * @note: 
     */
    retvalue = register_chrdev(CHRDEVBASE_MAJOR, CHRDEVBASE_NAME, &chrdevbase_fops);
    if(retvalue < 0)
    {
        printk("chrdevbase driver register failed\r\n");
    }
    
	printk("chrdevbase init!\r\n");
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
static void __exit chrdevbase_exit(void)
{
    /**
     * @function: 注销字符设备驱动
     * @parameter: 
     *      CHRDEVBASE_MAJOR：要注销的设备对应的主设备号
     *      CHRDEVBASE_NAME：要注销的设备对应的设备名
     * @return: 
     *     success: 
     *     error:
     * @note: 
     */   
	unregister_chrdev(CHRDEVBASE_MAJOR, CHRDEVBASE_NAME);
	printk("chrdevbase exit!\r\n");
}

/* 
 * 将上面两个函数指定为驱动的入口和出口函数 
 */
module_init(chrdevbase_init);
module_exit(chrdevbase_exit);

/* 
 * LICENSE和作者信息
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("cpucode");