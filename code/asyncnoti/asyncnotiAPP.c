/*
 * @Author: cpu_code
 * @Date: 2020-06-03 10:10:02
 * @LastEditTime: 2020-06-03 17:28:53
 * @FilePath: \Linux_driver\code\asyncnoti\asyncontiAPP.c
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
#include "poll.h"
#include "sys/select.h"
#include "sys/time.h"
#include "linux/ioctl.h"
#include "signal.h"

static int fd = 0;	/* 文件描述符 */

/**
 * @function: SIGIO信号处理函数
 * @parameter: 
 *      signum 	: 信号值
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static void sigio_signal_func(int signum)
{
    int err = 0;
	unsigned int keyvalue = 0;

    err = read(fd, &keyvalue, sizeof(keyvalue));

    if(err < 0)
    {
        /* 读取错误 */
    }
    else
    {
        printf("sigio signal! key value=%d\r\n", keyvalue);
    }
}

int main(int argc, char *argv[])
{
    int flags = 0;
	char *filename;
    if(argc != 2)
    {
        printf("Error Usage!\r\n");
        return -1;
    }

    filename = argv[1];
    fd = open(filename, O_RDWR);
    if(fd < 0)
    {
        printf("Can't open file %s\r\n", filename);
		return -1;
    }

    /* 设置信号SIGIO的处理函数 */
	signal(SIGIO, sigio_signal_func);

    fcntl(fd, F_SETOWN, getpid());		/* 设置当前进程接收SIGIO信号 	*/

    flags = fcntl(fd, F_GETFD);			/* 获取当前的进程状态 			*/

    fcntl(fd, F_SETFL, flags | FASYNC);	/* 设置进程启用异步通知功能 	*/	

    while(1) 
    {
		sleep(2);
	}

    close(fd);
	return 0;
}