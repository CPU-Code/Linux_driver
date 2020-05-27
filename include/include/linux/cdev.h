/*
 * @Author: cpu_code
 * @Date: 2020-05-17 09:41:44
 * @LastEditTime: 2020-05-19 22:25:16
 * @FilePath: \Linux_driver\include\include\linux\cdev.h
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */ 
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_CDEV_H
#define _LINUX_CDEV_H

#include <linux/kobject.h>
#include <linux/kdev_t.h>
#include <linux/list.h>
#include <linux/device.h>

struct file_operations;
struct inode;
struct module;

struct cdev 
{
	struct kobject 				 kobj;
	struct module 				 *owner;
	const struct file_operations *ops;		/* 字符设备文件操作函数集合 */
	struct list_head 			 list;
	dev_t 						 dev;		/* 设备号 */
	unsigned int 				 count;
} __randomize_layout;

/**
 * @function: 对其进行初始化
 * @parameter: 
 * 		cdev：要初始化的 cdev 结构体变量
 * 		file_operations：字符设备文件操作函数集合
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
void cdev_init(struct cdev *, const struct file_operations *);

struct cdev *cdev_alloc(void);

void cdev_put(struct cdev *p);

/**
 * @function: 向 Linux 系统添加字符设备(cdev 结构体变量)，
 * @parameter: 
 * 		cdev:指向要添加的字符设备(cdev 结构体变量)
 * 		dev_t:设备所使用的设备号
 * 		unsigned:要添加的设备数量
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
int cdev_add(struct cdev *, dev_t, unsigned);

void cdev_set_parent(struct cdev *p, struct kobject *kobj);
int cdev_device_add(struct cdev *cdev, struct device *dev);
void cdev_device_del(struct cdev *cdev, struct device *dev);

/**
 * @function: 从 Linux 内核中删除相应的字符设备
 * @parameter: 
 * 		cdev: 要删除的字符设备
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
void cdev_del(struct cdev *);

void cd_forget(struct inode *);

#endif
