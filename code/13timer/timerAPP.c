/*
 * @Author: cpu_code
 * @Date: 2020-05-26 15:55:46
 * @LastEditTime: 2020-05-27 12:53:58
 * @FilePath: \Linux_driver\code\13timer\timerAPP.c
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
#include "linux/ioctl.h"

/* 命令值 */
#define CLOSE_CMD 		(_IO(0XEF, 0x1))	/* 关闭定时器 */
#define OPEN_CMD		(_IO(0XEF, 0x2))	/* 打开定时器 */
#define SETPERIOD_CMD	(_IO(0XEF, 0x3))	/* 设置定时器周期命令 */

int main(int argc, char *argv[])
{
    int fd, ret;
	char *filename;
	unsigned int cmd;
	unsigned int arg;
	unsigned char str[100];

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

    while (1)
    {
        printf("Input CMD:");
        ret = scanf("%d", &cmd);
        if(ret != 1)
        {
            /* 参数输入错误 */
            gets(str);				/* 防止卡死 */
        }

        if(cmd == 1)				/* 关闭LED灯 */
        {
            cmd = CLOSE_CMD;
        }
        else if(cmd == 2)
        {
            /* 打开LED灯 */
            cmd = OPEN_CMD;
        }
        else if(cmd == 3)
        {
            cmd = SETPERIOD_CMD;	/* 设置周期值 */
            printf("Input Timer Period:");

            ret = scanf("%d", &arg);

            if(ret != 1)
            {
                /* 参数输入错误 */
                gets(str);			/* 防止卡死 */
            }
        }
        
        ioctl(fd, cmd, arg);		/* 控制定时器的打开和关闭 */
    }

    close(fd);
}