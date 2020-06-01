/*
 * @Author: cpu_code
 * @Date: 2020-05-27 14:12:51
 * @LastEditTime: 2020-06-01 15:56:58
 * @FilePath: \Linux_driver\code\14irq\irqAPP.c
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

int main(int argc, char *argv[])
{
    int fd;
	int ret = 0;
	int data = 0;
	char *filename;
	unsigned char data;

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
        ret = read(fd, &data, sizeof(data));
        if(ret < 0)
        {
            /* 数据读取错误或者无效 */
        }
        else
        {
            /* 数据读取正确 */
            if(data)	/* 读取到数据 */
            {
                printf("key value = %#X\r\n", data);
            }
        }
    }

    close(fd);
	return ret;
}
