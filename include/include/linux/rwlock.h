#ifndef __LINUX_RWLOCK_H
#define __LINUX_RWLOCK_H

#ifndef __LINUX_SPINLOCK_H
# error "please don't include this file directly"
#endif

/*
 * rwlock related methods
 *
 * split out from spinlock.h
 *
 * portions Copyright 2005, Red Hat, Inc., Ingo Molnar
 * Released under the General Public License (GPL).
 */

#ifdef CONFIG_DEBUG_SPINLOCK
  extern void __rwlock_init(rwlock_t *lock, const char *name,
			    struct lock_class_key *key);
# define rwlock_init(lock)					\
do {								\
	static struct lock_class_key __key;			\
								\
	__rwlock_init((lock), #lock, &__key);			\
} while (0)
#else

/**
 * @function: 初始化读写锁
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
# define rwlock_init(lock)					\
	do { *(lock) = __RW_LOCK_UNLOCKED(lock); } while (0)
#endif

#ifdef CONFIG_DEBUG_SPINLOCK
 extern void do_raw_read_lock(rwlock_t *lock) __acquires(lock);
#define do_raw_read_lock_flags(lock, flags) do_raw_read_lock(lock)
 extern int do_raw_read_trylock(rwlock_t *lock);
 extern void do_raw_read_unlock(rwlock_t *lock) __releases(lock);
 extern void do_raw_write_lock(rwlock_t *lock) __acquires(lock);
#define do_raw_write_lock_flags(lock, flags) do_raw_write_lock(lock)
 extern int do_raw_write_trylock(rwlock_t *lock);
 extern void do_raw_write_unlock(rwlock_t *lock) __releases(lock);
#else

#ifndef arch_read_lock_flags
# define arch_read_lock_flags(lock, flags)	arch_read_lock(lock)
#endif

#ifndef arch_write_lock_flags
# define arch_write_lock_flags(lock, flags)	arch_write_lock(lock)
#endif

# define do_raw_read_lock(rwlock)	do {__acquire(lock); arch_read_lock(&(rwlock)->raw_lock); } while (0)
# define do_raw_read_lock_flags(lock, flags) \
		do {__acquire(lock); arch_read_lock_flags(&(lock)->raw_lock, *(flags)); } while (0)
# define do_raw_read_trylock(rwlock)	arch_read_trylock(&(rwlock)->raw_lock)
# define do_raw_read_unlock(rwlock)	do {arch_read_unlock(&(rwlock)->raw_lock); __release(lock); } while (0)
# define do_raw_write_lock(rwlock)	do {__acquire(lock); arch_write_lock(&(rwlock)->raw_lock); } while (0)
# define do_raw_write_lock_flags(lock, flags) \
		do {__acquire(lock); arch_write_lock_flags(&(lock)->raw_lock, *(flags)); } while (0)
# define do_raw_write_trylock(rwlock)	arch_write_trylock(&(rwlock)->raw_lock)
# define do_raw_write_unlock(rwlock)	do {arch_write_unlock(&(rwlock)->raw_lock); __release(lock); } while (0)
#endif

/*
 * Define the various rw_lock methods.  Note we define these
 * regardless of whether CONFIG_SMP or CONFIG_PREEMPT are set. The various
 * methods are defined as nops in the case they are not required.
 */
#define read_trylock(lock)	__cond_lock(lock, _raw_read_trylock(lock))
#define write_trylock(lock)	__cond_lock(lock, _raw_write_trylock(lock))

/**
 * @function: 获取写锁
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
#define write_lock(lock)	_raw_write_lock(lock)

/**
 * @function: 获取读锁
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
#define read_lock(lock)		_raw_read_lock(lock)

#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK)

#define read_lock_irqsave(lock, flags)			\
	do {						\
		typecheck(unsigned long, flags);	\
		flags = _raw_read_lock_irqsave(lock);	\
	} while (0)
#define write_lock_irqsave(lock, flags)			\
	do {						\
		typecheck(unsigned long, flags);	\
		flags = _raw_write_lock_irqsave(lock);	\
	} while (0)

#else

/**
 * @function: 保存中断状态，禁止本地中断，并获取读锁
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
#define read_lock_irqsave(lock, flags)			\
	do {						\
		typecheck(unsigned long, flags);	\
		_raw_read_lock_irqsave(lock, flags);	\
	} while (0)

/**
 * @function: 保存中断状态，禁止本地中断，并获取写锁
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
#define write_lock_irqsave(lock, flags)			\
	do {						\
		typecheck(unsigned long, flags);	\
		_raw_write_lock_irqsave(lock, flags);	\
	} while (0)

#endif

/**
 * @function: 禁止本地中断，并且获取读锁
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
#define read_lock_irq(lock)		_raw_read_lock_irq(lock)

/**
 * @function: 关闭下半部，并获取读锁
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
#define read_lock_bh(lock)		_raw_read_lock_bh(lock)

/**
 * @function: 禁止本地中断，并且获取写锁
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
#define write_lock_irq(lock)		_raw_write_lock_irq(lock)

/**
 * @function: 关闭下半部，并获取读锁
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
#define write_lock_bh(lock)		_raw_write_lock_bh(lock)

/**
 * @function: 释放读锁
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
#define read_unlock(lock)		_raw_read_unlock(lock)

/**
 * @function: 释放写锁
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
#define write_unlock(lock)		_raw_write_unlock(lock)

/**
 * @function: 打开本地中断，并且释放读锁
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
#define read_unlock_irq(lock)		_raw_read_unlock_irq(lock)

/**
 * @function: 打开本地中断，并且释放写锁
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
#define write_unlock_irq(lock)		_raw_write_unlock_irq(lock)

/**
 * @function: 将中断状态恢复到以前的状态，并且激活本地中断，释放读锁
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
#define read_unlock_irqrestore(lock, flags)			\
	do {							\
		typecheck(unsigned long, flags);		\
		_raw_read_unlock_irqrestore(lock, flags);	\
	} while (0)

/**
 * @function: 打开下半部，并释放读锁
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
#define read_unlock_bh(lock)		_raw_read_unlock_bh(lock)

/**
 * @function: 将中断状态恢复到以前的状态，并且激活本地中断，释放读锁
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
#define write_unlock_irqrestore(lock, flags)		\
	do {						\
		typecheck(unsigned long, flags);	\
		_raw_write_unlock_irqrestore(lock, flags);	\
	} while (0)

/**
 * @function: 打开下半部，并释放读锁
 * @parameter: 
 * @return: 
 *     success: 
 *     error:
 * @note: 
 */
#define write_unlock_bh(lock)		_raw_write_unlock_bh(lock)

#define write_trylock_irqsave(lock, flags) \
({ \
	local_irq_save(flags); \
	write_trylock(lock) ? \
	1 : ({ local_irq_restore(flags); 0; }); \
})

#endif /* __LINUX_RWLOCK_H */
