/*
 * @Author: cpu_code
 * @Date: 2020-05-17 09:42:03
 * @LastEditTime: 2020-05-25 21:18:02
 * @FilePath: \Linux_driver\include\include\linux\semaphore.h
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */ 
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2008 Intel Corporation
 * Author: Matthew Wilcox <willy@linux.intel.com>
 *
 * Please see kernel/locking/semaphore.c for documentation of these functions
 */
#ifndef __LINUX_SEMAPHORE_H
#define __LINUX_SEMAPHORE_H

#include <linux/list.h>
#include <linux/spinlock.h>

/* Please don't access any members of this structure directly */
/* 信号量 */
struct semaphore {
	raw_spinlock_t		lock;
	unsigned int		count;
	struct list_head	wait_list;
};

#define __SEMAPHORE_INITIALIZER(name, n)				\
{									\
	.lock		= __RAW_SPIN_LOCK_UNLOCKED((name).lock),	\
	.count		= n,						\
	.wait_list	= LIST_HEAD_INIT((name).wait_list),		\
}

#define DEFINE_SEMAPHORE(name)	\
	struct semaphore name = __SEMAPHORE_INITIALIZER(name, 1)

/**
 * @function: 初始化信号量 sem，设置信号量值为 val
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static inline void sema_init(struct semaphore *sem, int val)
{
	static struct lock_class_key __key;
	*sem = (struct semaphore) __SEMAPHORE_INITIALIZER(*sem, val);
	lockdep_init_map(&sem->lock.dep_map, "semaphore->lock", &__key, 0);
}

/**
 * @function: 获取信号量，因为会导致休眠，因此不能在中断中使用
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
extern void down(struct semaphore *sem);

/**
 * @function: 获取信号量，和 down 类似，只是使用 down 进入休眠状态的线程不能被信号打断
 * 				而使用此函数进入休眠以后是可以被信号打断的
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
extern int __must_check down_interruptible(struct semaphore *sem);
extern int __must_check down_killable(struct semaphore *sem);

/**
 * @function: 尝试获取信号量，如 能获取到信号量就获取，并且返回 0。如 不能就返回非 0，并且不会进入休眠
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
extern int __must_check down_trylock(struct semaphore *sem);
extern int __must_check down_timeout(struct semaphore *sem, long jiffies);

/**
 * @function: 释放信号量
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
extern void up(struct semaphore *sem);

#endif /* __LINUX_SEMAPHORE_H */
