/*
 * @Author: cpu_code
 * @Date: 2020-06-04 22:57:14
 * @LastEditTime: 2020-06-05 09:33:32
 * @FilePath: \Linux_driver\code\miscbeep\miscbeepAPP.c
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

#define BEEPOFF	0
#define BEEPON 	1

int main(int argc, char *argv[])
{
    int fd, retvalue;
	char *filename;
	unsigned char databuf[1];

    if(argc != 3)
    {
        printf("Error Usage!\r\n");
		return -1;
    }

    databuf[0] = atoi(argv[2]);	/* 要执行的操作：打开或关闭 */
    retvalue = write(fd, databuf, sizeof(databuf));
    if(retvalue < 0)
    {
        printf("BEEP Control Failed!\r\n");
		close(fd);
		return -1;
    }

    retvalue = close(fd); /* 关闭文件 */
    if(retvalue < 0)
    {
        printf("file %s close failed!\r\n", argv[1]);
		return -1;
    }

    return 0;
}