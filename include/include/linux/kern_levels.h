/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __KERN_LEVELS_H__
#define __KERN_LEVELS_H__

#define KERN_SOH	"\001"		/* ASCII Start Of Header */
#define KERN_SOH_ASCII	'\001'

/* printk 可以根据日志级别对消息进行分类 */
/*  0 的优先级最高， 7 的优先级最低 */
/* 
 * 要设置消息级别，参考如下示例：
 * printk(KERN_EMERG "gsmi: Log Shutdown Reason\n");
 */
/* printk 将 会 采 用 默 认 级 别 MESSAGE_LOGLEVEL_DEFAULT */
/*  include/linux/printk.h  */
#define KERN_EMERG	KERN_SOH "0"	/* 紧急事件，一般是内核崩溃 */
#define KERN_ALERT	KERN_SOH "1"	/* 必须立即采取行动 */
#define KERN_CRIT	KERN_SOH "2"	/*  临界条件，比如严重的软件或硬件错误 */
#define KERN_ERR	KERN_SOH "3"	/* 错误状态，一般设备驱动程序中使用KERN_ERR 报告硬件错误 */
#define KERN_WARNING	KERN_SOH "4"	/* 警告信息，不会对系统造成严重影响 */
#define KERN_NOTICE	KERN_SOH "5"	/* 有必要进行提示的一些信息 */
#define KERN_INFO	KERN_SOH "6"	/* 提示性的信息 */
#define KERN_DEBUG	KERN_SOH "7"	/* 调试信息 */

#define KERN_DEFAULT	""		/* the default kernel loglevel */

/*
 * Annotation for a "continued" line of log printout (only done after a
 * line that had no enclosing \n). Only to be used by core/arch code
 * during early bootup (a continued line is not SMP-safe otherwise).
 */
#define KERN_CONT	KERN_SOH "c"

/* integer equivalents of KERN_<LEVEL> */
#define LOGLEVEL_SCHED		-2	/* Deferred messages from sched code
					 * are set to this special level */
#define LOGLEVEL_DEFAULT	-1	/* default (or last) loglevel */
#define LOGLEVEL_EMERG		0	/* system is unusable */
#define LOGLEVEL_ALERT		1	/* action must be taken immediately */
#define LOGLEVEL_CRIT		2	/* critical conditions */
#define LOGLEVEL_ERR		3	/* error conditions */
#define LOGLEVEL_WARNING	4	/* warning conditions */
#define LOGLEVEL_NOTICE		5	/* normal but significant condition */
#define LOGLEVEL_INFO		6	/* informational */
#define LOGLEVEL_DEBUG		7	/* debug-level messages */

#endif
