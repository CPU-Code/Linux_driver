/*
 * @Author: cpu_code
 * @Date: 2020-05-17 09:41:30
 * @LastEditTime: 2020-05-24 17:18:39
 * @FilePath: \Linux_driver\include\include\asm-generic\bitops\atomic.h
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */ 
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_GENERIC_BITOPS_ATOMIC_H_
#define _ASM_GENERIC_BITOPS_ATOMIC_H_

#include <linux/atomic.h>
#include <linux/compiler.h>
#include <asm/barrier.h>

/*
 * Implementation of atomic bitops using atomic-fetch ops.
 * See Documentation/atomic_bitops.txt for details.
 */

/**
 * @function: 将 p 地址的第 nr 位置 1
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static inline void set_bit(unsigned int nr, volatile unsigned long *p)
{
	p += BIT_WORD(nr);
	atomic_long_or(BIT_MASK(nr), (atomic_long_t *)p);
}

/**
 * @function: 将 p 地址的第 nr 位清零
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static inline void clear_bit(unsigned int nr, volatile unsigned long *p)
{
	p += BIT_WORD(nr);
	atomic_long_andnot(BIT_MASK(nr), (atomic_long_t *)p);
}

/**
 * @function: 将 p 地址的第 nr 位进行翻转
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static inline void change_bit(unsigned int nr, volatile unsigned long *p)
{
	p += BIT_WORD(nr);
	atomic_long_xor(BIT_MASK(nr), (atomic_long_t *)p);
}

/* int test_bit(int nr, void *p) 获取 p 地址的第 nr 位的值 */

/**
 * @function: 将 p 地址的第 nr 位置 1，并且返回 nr 位原来的值
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static inline int test_and_set_bit(unsigned int nr, volatile unsigned long *p)
{
	long old;
	unsigned long mask = BIT_MASK(nr);

	p += BIT_WORD(nr);
	if (READ_ONCE(*p) & mask)
		return 1;

	old = atomic_long_fetch_or(mask, (atomic_long_t *)p);
	return !!(old & mask);
}

/**
 * @function: 将 p 地址的第 nr 位清零，并且返回 nr 位原来的值
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static inline int test_and_clear_bit(unsigned int nr, volatile unsigned long *p)
{
	long old;
	unsigned long mask = BIT_MASK(nr);

	p += BIT_WORD(nr);
	if (!(READ_ONCE(*p) & mask))
		return 0;

	old = atomic_long_fetch_andnot(mask, (atomic_long_t *)p);
	return !!(old & mask);
}

/**
 * @function: 将 p 地址的第 nr 位翻转，并且返回 nr 位原来的值
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
static inline int test_and_change_bit(unsigned int nr, volatile unsigned long *p)
{
	long old;
	unsigned long mask = BIT_MASK(nr);

	p += BIT_WORD(nr);
	old = atomic_long_fetch_xor(mask, (atomic_long_t *)p);
	return !!(old & mask);
}

#endif /* _ASM_GENERIC_BITOPS_ATOMIC_H */
