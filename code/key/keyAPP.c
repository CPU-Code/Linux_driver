/*
 * @Author: cpu_code
 * @Date: 2020-05-26 10:37:44
 * @LastEditTime: 2020-05-26 15:46:20
 * @FilePath: \Linux_driver\code\12key\keyAPP.c
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

/* 定义按键值 */
#define KEY0VALUE	0XF0
#define INVAKEY		0X00

int main(int argc, char *argv[])
{
    int fd, ret;
	char *filename;
	int keyvalue;

    if(argc != 2)
    {
        printf("Error Usage!\r\n");
        return -1;
    }

    filename = argv[1];

    /* 打开key驱动 */
	fd = open(filename, O_RDWR);
    if(fd < 0)
    {
        printf("file %s open failed!\r\n", argv[1]);
        return -1;
    }
    
    /* 循环读取按键值数据！ */
    while(1)
    {
        read(fd, &keyvalue, sizeof(keyvalue));
        if(keyvalue == KEY0VALUE)
        {
            /* KEY0 */
            printf("KEY0 Press, value = %#X\r\n", keyvalue);	/* 按下 */
        }
    }

    ret= close(fd); /* 关闭文件 */
    if(ret < 0)
    {
        printf("file %s close failed!\r\n", argv[1]);
		return -1;
    }

    return 0;
}