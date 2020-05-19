/**
 * @Author: cpu_code
 * @Date: 2020-05-17 09:41:59
 * @LastEditTime: 2020-05-19 09:49:14
 * @FilePath: \Linux_driver\include\include\linux\of_address.h
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */ 

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __OF_ADDRESS_H
#define __OF_ADDRESS_H
#include <linux/ioport.h>
#include <linux/errno.h>
#include <linux/of.h>
#include <linux/io.h>

struct of_pci_range_parser {
	struct device_node *node;
	const __be32 *range;
	const __be32 *end;
	int np;
	int pna;
	bool dma;
};

struct of_pci_range {
	u32 pci_space;
	u64 pci_addr;
	u64 cpu_addr;
	u64 size;
	u32 flags;
};

#define for_each_of_pci_range(parser, range) \
	for (; of_pci_range_parser_one(parser, range);)

/* Translate a DMA address from device space to CPU space */
extern u64 of_translate_dma_address(struct device_node *dev,
				    const __be32 *in_addr);

#ifdef CONFIG_OF_ADDRESS

/**
 * @function: 将从设备树读取到的地址转换为物理地址
 * @parameter: 
 * 		np：设备节点
 * 		addr：要转换的地址
 * @return: 
 *     success: 得到的物理地址
 *     error: OF_BAD_ADDR 
 * @note: 
 */
extern u64 of_translate_address(struct device_node *np, const __be32 *addr);

/**
 * @function: 将 reg 属性值，然后将其转换为 resource 结构体类型
 * @parameter: 
 * 		dev：设备节点
 * 		index：地址资源标号
 * 		r：得到的 resource 类型的资源值
 * @return: 
 *     success: 0
 *     error: 负值
 * @note: 
 */
extern int of_address_to_resource(struct device_node *dev, int index,
				  struct resource *r);

/**
 * @function: 将 reg 属性中地址信息转换为虚拟地址，
 * 				如 reg 属性有多段的话，可通过 index 参数指定要完成内存映射的是哪一段
 * @parameter:
 * 		device：设备节点
 * 		index：reg 属性中要完成内存映射的段，如 reg 属性只有一段的话 index 就设置为 0
 * @return: 
 *     success: 经过内存映射后的虚拟内存首地址
 *     error: NULL
 * @note: 
 */
extern void __iomem *of_iomap(struct device_node *device, int index);

void __iomem *of_io_request_and_map(struct device_node *device,
				    int index, const char *name);

/* Extract an address from a device, returns the region size and
 * the address space flags too. The PCI version uses a BAR number
 * instead of an absolute index
 */
/**
 * @function: 用于获取地址相关属性，主要是“ reg ”或 “ assigned-addresses ”属性值
 * @parameter:
 * 		dev：设备节点
 * 		index： 要读取的地址标号
 * 		size：地址长度
 * 		flags：参数，如 IORESOURCE_IO、 IORESOURCE_MEM 
 * @return: 
 *     success: 读取到的地址数据首地址
 *     error: NULL
 * @note: 
 */
extern const __be32 *of_get_address(struct device_node *dev, int index,
			   u64 *size, unsigned int *flags);

extern int of_pci_range_parser_init(struct of_pci_range_parser *parser,
			struct device_node *node);
extern int of_pci_dma_range_parser_init(struct of_pci_range_parser *parser,
			struct device_node *node);
extern struct of_pci_range *of_pci_range_parser_one(
					struct of_pci_range_parser *parser,
					struct of_pci_range *range);
extern bool of_dma_is_coherent(struct device_node *np);
#else /* CONFIG_OF_ADDRESS */
static inline void __iomem *of_io_request_and_map(struct device_node *device,
						  int index, const char *name)
{
	return IOMEM_ERR_PTR(-EINVAL);
}

static inline u64 of_translate_address(struct device_node *np,
				       const __be32 *addr)
{
	return OF_BAD_ADDR;
}

static inline const __be32 *of_get_address(struct device_node *dev, int index,
					u64 *size, unsigned int *flags)
{
	return NULL;
}

static inline int of_pci_range_parser_init(struct of_pci_range_parser *parser,
			struct device_node *node)
{
	return -ENOSYS;
}

static inline int of_pci_dma_range_parser_init(struct of_pci_range_parser *parser,
			struct device_node *node)
{
	return -ENOSYS;
}

static inline struct of_pci_range *of_pci_range_parser_one(
					struct of_pci_range_parser *parser,
					struct of_pci_range *range)
{
	return NULL;
}

static inline bool of_dma_is_coherent(struct device_node *np)
{
	return false;
}
#endif /* CONFIG_OF_ADDRESS */

#ifdef CONFIG_OF
extern int of_address_to_resource(struct device_node *dev, int index,
				  struct resource *r);
void __iomem *of_iomap(struct device_node *node, int index);
#else
static inline int of_address_to_resource(struct device_node *dev, int index,
					 struct resource *r)
{
	return -EINVAL;
}

static inline void __iomem *of_iomap(struct device_node *device, int index)
{
	return NULL;
}
#endif

#if defined(CONFIG_OF_ADDRESS) && defined(CONFIG_PCI)
extern const __be32 *of_get_pci_address(struct device_node *dev, int bar_no,
			       u64 *size, unsigned int *flags);
extern int of_pci_address_to_resource(struct device_node *dev, int bar,
				      struct resource *r);
extern int of_pci_range_to_resource(struct of_pci_range *range,
				    struct device_node *np,
				    struct resource *res);
#else /* CONFIG_OF_ADDRESS && CONFIG_PCI */
static inline int of_pci_address_to_resource(struct device_node *dev, int bar,
				             struct resource *r)
{
	return -ENOSYS;
}

static inline const __be32 *of_get_pci_address(struct device_node *dev,
		int bar_no, u64 *size, unsigned int *flags)
{
	return NULL;
}
static inline int of_pci_range_to_resource(struct of_pci_range *range,
					   struct device_node *np,
					   struct resource *res)
{
	return -ENOSYS;
}
#endif /* CONFIG_OF_ADDRESS && CONFIG_PCI */

#endif /* __OF_ADDRESS_H */

