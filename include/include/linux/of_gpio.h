/*
 * @Author: cpu_code
 * @Date: 2020-05-17 09:41:59
 * @LastEditTime: 2020-05-20 11:18:43
 * @FilePath: \Linux_driver\include\include\linux\of_gpio.h
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */ 
/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * OF helpers for the GPIO API
 *
 * Copyright (c) 2007-2008  MontaVista Software, Inc.
 *
 * Author: Anton Vorontsov <avorontsov@ru.mvista.com>
 */

#ifndef __LINUX_OF_GPIO_H
#define __LINUX_OF_GPIO_H

#include <linux/compiler.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/of.h>

struct device_node;

/*
 * This is Linux-specific flags. By default controllers' and Linux' mapping
 * match, but GPIO controllers are free to translate their own flags to
 * Linux-specific in their .xlate callback. Though, 1:1 mapping is recommended.
 */
enum of_gpio_flags {
	OF_GPIO_ACTIVE_LOW = 0x1,
	OF_GPIO_SINGLE_ENDED = 0x2,
	OF_GPIO_OPEN_DRAIN = 0x4,
	OF_GPIO_TRANSITORY = 0x8,
	OF_GPIO_PULL_UP = 0x10,
	OF_GPIO_PULL_DOWN = 0x20,
};

#ifdef CONFIG_OF_GPIO

/*
 * OF GPIO chip for memory mapped banks
 */
struct of_mm_gpio_chip {
	struct gpio_chip gc;
	void (*save_regs)(struct of_mm_gpio_chip *mm_gc);
	void __iomem *regs;
};

static inline struct of_mm_gpio_chip *to_of_mm_gpio_chip(struct gpio_chip *gc)
{
	return container_of(gc, struct of_mm_gpio_chip, gc);
}

extern int of_get_named_gpio_flags(struct device_node *np,
		const char *list_name, int index, enum of_gpio_flags *flags);

extern int of_mm_gpiochip_add_data(struct device_node *np,
				   struct of_mm_gpio_chip *mm_gc,
				   void *data);
static inline int of_mm_gpiochip_add(struct device_node *np,
				     struct of_mm_gpio_chip *mm_gc)
{
	return of_mm_gpiochip_add_data(np, mm_gc, NULL);
}
extern void of_mm_gpiochip_remove(struct of_mm_gpio_chip *mm_gc);

#else /* CONFIG_OF_GPIO */

/* Drivers may not strictly depend on the GPIO support, so let them link. */
static inline int of_get_named_gpio_flags(struct device_node *np,
		const char *list_name, int index, enum of_gpio_flags *flags)
{
	if (flags)
		*flags = 0;

	return -ENOSYS;
}

#endif /* CONFIG_OF_GPIO */

/**
 * of_gpio_named_count() - Count GPIOs for a device
 * @np:		device node to count GPIOs for
 * @propname:	property name containing gpio specifier(s)
 *
 * The function returns the count of GPIOs specified for a node.
 * Note that the empty GPIO specifiers count too. Returns either
 *   Number of gpios defined in property,
 *   -EINVAL for an incorrectly formed gpios property, or
 *   -ENOENT for a missing gpios property
 *
 * Example:
 * gpios = <0
 *          &gpio1 1 2
 *          0
 *          &gpio2 3 4>;
 *
 * The above example defines four GPIOs, two of which are not specified.
 * This function will return '4'
 */

/**
 * @function: 获取设备树某个属性里面定义了几个 GPIO 信息，空的 GPIO 信息也会被统计到
 * @parameter: 
 * 		np：设备节点
 * 		propname：要统计的 GPIO 属性
 * @return: 
 *     success: 正值，统计到的 GPIO 数量
 *     error: 负值
 * @note: 
 */
static inline int of_gpio_named_count(struct device_node *np, const char* propname)
{
	return of_count_phandle_with_args(np, propname, "#gpio-cells");
}

/**
 * of_gpio_count() - Count GPIOs for a device
 * @np:		device node to count GPIOs for
 *
 * Same as of_gpio_named_count, but hard coded to use the 'gpios' property
 */
/**
 * @function: 统计的是“gpios”这个属性的 GPIO 数量
 * @parameter: 
 * 		np：设备节点
 * @return: 
 *     success: 正值，统计到的 GPIO 数量
 *     error: 负值
 * @note: 
 */
static inline int of_gpio_count(struct device_node *np)
{
	return of_gpio_named_count(np, "gpios");
}

static inline int of_get_gpio_flags(struct device_node *np, int index,
		      enum of_gpio_flags *flags)
{
	return of_get_named_gpio_flags(np, "gpios", index, flags);
}

/**
 * of_get_named_gpio() - Get a GPIO number to use with GPIO API
 * @np:		device node to get GPIO from
 * @propname:	Name of property containing gpio specifier(s)
 * @index:	index of the GPIO
 *
 * Returns GPIO number to use with Linux generic GPIO API, or one of the errno
 * value on the error condition.*
 */
/**
 * @function: 获取 GPIO 编号
 * @parameter: 
 * 		np：设备节点
 * 		propname：包含要获取 GPIO 信息的属性名
 * 		index： GPIO 索引，如 只有一个 GPIO 信息的话此参数为 0
 * @return: 
 *     success: 正值，获取到的 GPIO 编号
 *     error:
 * @note: 负值
 */
static inline int of_get_named_gpio(struct device_node *np,
                                   const char *propname, int index)
{
	return of_get_named_gpio_flags(np, propname, index, NULL);
}

/**
 * of_get_gpio() - Get a GPIO number to use with GPIO API
 * @np:		device node to get GPIO from
 * @index:	index of the GPIO
 *
 * Returns GPIO number to use with Linux generic GPIO API, or one of the errno
 * value on the error condition.
 */
static inline int of_get_gpio(struct device_node *np, int index)
{
	return of_get_gpio_flags(np, index, NULL);
}

#endif /* __LINUX_OF_GPIO_H */
