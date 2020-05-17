/*
 * @Author: cpu_code
 * @Date: 2020-05-16 21:43:02
 * @LastEditTime: 2020-05-17 09:45:08
 * @FilePath: \Linux_driver\chrdevbase\chrdevbase.c
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
    memcpy(readbuf, kerneldata, sizeof(kerneldata));

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