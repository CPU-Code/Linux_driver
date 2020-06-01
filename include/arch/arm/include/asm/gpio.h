/*
 * @Author: cpu_code
 * @Date: 2020-05-17 09:48:08
 * @LastEditTime: 2020-05-28 09:49:50
 * @FilePath: \Linux_driver\include\arch\arm\include\asm\gpio.h
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */ 
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ARCH_ARM_GPIO_H
#define _ARCH_ARM_GPIO_H

#if CONFIG_ARCH_NR_GPIO > 0
#define ARCH_NR_GPIOS CONFIG_ARCH_NR_GPIO
#endif

/* Note: this may rely upon the value of ARCH_NR_GPIOS set in mach/gpio.h */
#include <asm-generic/gpio.h>

/* The trivial gpiolib dispatchers */
/* 获取某个 GPIO 的值 */
#define gpio_get_value  __gpio_get_value
/* 设置某个 GPIO 的值 */
#define gpio_set_value  __gpio_set_value
#define gpio_cansleep   __gpio_cansleep

/*
 * Provide a default gpio_to_irq() which should satisfy every case.
 * However, some platforms want to do this differently, so allow them
 * to override it.
 */
#ifndef gpio_to_irq
/**
 * @function: 获取 gpio 对应的中断号
 * @parameter: 
 *      gpio： 要获取的 GPIO 编号
 * @return: 
 *     success: GPIO 对应的中断号
 *     error:
 * @note: 
 */
#define gpio_to_irq	__gpio_to_irq
#endif

#endif /* _ARCH_ARM_GPIO_H */
