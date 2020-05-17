/*
 * @Author: cpu_code
 * @Date: 2020-05-17 09:56:54
 * @LastEditTime: 2020-05-17 14:15:08
 * @FilePath: \Linux_driver\1chrdevbase\chrdevbaseAPP.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */ 

#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

//测试 APP 要向 chrdevbase 设备写入的数据
static char usrdata[] = {"usr data!"};

/**
 * @function:  字符设备驱动的开发步骤
 * @parameter: 
 *      argc: argv数组元素个数
 *      argv: 具体参数
 * @return: 
 *     success: 0
 *     error: 其他
 * @note: 
 */
int main(int argc, char *argv[])
{
    int fd, retvalue;
    char *filename;
    char readbuf[100], writebuf[100];

    //参数不为 3 个的话就表示测试 APP 用法错误
    if(argc != 3)
    {
        printf("Error Usage!\r\n");
        return -1;
    }

    //获取要打开的设备文件名字
    filename = argv[1];

    /**
     * @function: 打开驱动文件
     * @parameter: 
     *      filename: 要打开的设备或者文件名
     *      O_RDWR: 读写模式
     *          O_RDONLY 只读模式
     *          O_WRONLY 只写模式
     *      逻辑或来选择多种模式：
     *          O_APPEND：每次写操作都写入文件的末尾
     *          O_CREAT：如 指定文件不存在，则创建这个文件
     *          O_EXCL：如 要创建的文件已存在，则返回 -1，并且修改 errno 的值
     *          O_TRUNC 如 文件存在，并且以只写/读写方式打开，则清空文件全部内容
     *          O_NOCTTY 如 路径名指向终端设备，不要把这个设备用作控制终端
     *          O_NONBLOCK：如 路径名指向 FIFO/块文件/字符文件，则把文件的打开和后继I/O 设置为非阻塞
     *          DSYNC： 等待物理 I/O 结束后再 write。在不影响读取新写入的数据的前提下，不等待文件属性更新
     *          O_RSYNC：read 等待所有写入同一区域的写操作完成后再进行
     *          O_SYNC：等待物理 I/O 结束后再 write，包括更新文件属性的 I/O
     * @return: 
     *     success: 文件的文件描述符
     *     error: 负值
     * @note: 
     */   
	fd  = open(filename, O_RDWR);
    if(fd < 0)
    {
        printf("Can't open file %s\r\n", filename);
        return -1;
    }

    /* 将字符串格式的数字转换为真实的数字 */
    //1表示从chrdevbase中读取数据
    if(atoi(argv[2]) == 1)
    {

        /**
         * @function: 从驱动文件读取数据 
         * @parameter: 
         *      fd：要读取的文件描述符
         *      readbuf：数据读取到此 readbuf 中
         *      50：要读取的数据长度，字节数
         * @return: 
         *     success: 读取到的字节数
         *     error:负值
         * @note:  0 表示读取到了文件末尾
         */
        retvalue = read(fd, readbuf, 50);
        if(retvalue < 0)
        {
            printf("read file %s failed!\r\n", filename);
        }
        else
        {
            /*  读取成功，打印出读取成功的数据 */
            printf("read data:%s\r\n",readbuf);
        }
    }

    //2 表示向 chrdevbase 写数据
    if(atoi(argv[2]) == 2)
    {
        memcpy(writebuf, usrdata, sizeof(usrdata));

        /**
         * @function: 向设备驱动写数据 
         * @parameter: 
         *      fd:要进行写操作的文件描述符
         *      writebuf: 要写入的数据
         *      50: 要写入的数据长度，字节数
         * @return: 
         *     success: 写入的字节数
         *     error:负值
         * @note: 0 表示没有写入任何数据
         */
        retvalue = write(fd, writebuf, 50);
        if(retvalue < 0)
        {
            printf("write file %s failed!\r\n", filename);
        }
    }
    
    /**
     * @function: 关闭设备
     * @parameter: 
     *      fd: 要关闭的文件描述符
     * @return: 
     *     success: 0
     *     error:负值
     * @note: 
     */   
	retvalue = close(fd);
    if(retvalue < 0)
    {
        printf("Can't close file %s\r\n", filename);
        return -1;
    }

    return 0;
}