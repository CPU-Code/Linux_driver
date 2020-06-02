/*
 * @Author: cpu_code
 * @Date: 2020-05-17 09:41:30
 * @LastEditTime: 2020-06-02 16:48:19
 * @FilePath: \Linux_driver\include\include\asm-generic\current.h
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */ 
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_GENERIC_CURRENT_H
#define __ASM_GENERIC_CURRENT_H

#include <linux/thread_info.h>

#define get_current() (current_thread_info()->task)
//current 相当于一个全局变量 ， 表示 当前进程
#define current get_current()

#endif /* __ASM_GENERIC_CURRENT_H */
