/*
 * @Author: cpu_code
 * @Date: 2020-05-17 09:41:47
 * @LastEditTime: 2020-05-27 20:51:46
 * @FilePath: \Linux_driver\include\include\linux\irqreturn.h
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */ 
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_IRQRETURN_H
#define _LINUX_IRQRETURN_H

/**
 * enum irqreturn
 * @IRQ_NONE		interrupt was not from this device or was not handled
 * @IRQ_HANDLED		interrupt was handled by this device
 * @IRQ_WAKE_THREAD	handler requests to wake the handler thread
 */
enum irqreturn {
	IRQ_NONE		= (0 << 0),
	IRQ_HANDLED		= (1 << 0),
	IRQ_WAKE_THREAD		= (1 << 1),
};
/* 枚举类型 */
typedef enum irqreturn irqreturn_t;
#define IRQ_RETVAL(x)	((x) ? IRQ_HANDLED : IRQ_NONE)

#endif
