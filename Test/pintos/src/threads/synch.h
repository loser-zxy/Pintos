#ifndef THREADS_SYNCH_H
#define THREADS_SYNCH_H

#include <list.h>
#include <stdbool.h>

/* A counting semaphore. */
struct semaphore
{
    unsigned value;             /* Current value. */
    struct list waiters;        /* List of waiting threads. */
};

void sema_init (struct semaphore *, unsigned value);
void sema_down (struct semaphore *);
bool sema_try_down (struct semaphore *);
void sema_up (struct semaphore *);
void sema_self_test (void);

/* Lock. */
struct lock
{
    struct thread *holder;      /* Thread holding lock (for debugging). */
    struct semaphore semaphore; /* Binary semaphore controlling access. */
};

void lock_init (struct lock *);
void lock_acquire (struct lock *);
bool lock_try_acquire (struct lock *);
void lock_release (struct lock *);
bool lock_held_by_current_thread (const struct lock *);

/* Condition variable. */
struct condition
{
    struct list waiters;        /* List of waiting threads. */
};

void cond_init (struct condition *);
void cond_wait (struct condition *, struct lock *);
void cond_signal (struct condition *, struct lock *);
void cond_broadcast (struct condition *, struct lock *);

/*
 * 定义一个比较线程priority高低的函数，
 * 用于semaphore -> waiters队列“从低到高”的排序
 */
bool priority_cmp_low_to_max(
    const struct list_elem *a,
    const struct list_elem *b,
    void *aux
);

/*
 * 定义一个比较线程priority高低的函数，
 * 用于semaphore -> waiters队列“从高到低”的排序
 */
bool priority_cmp_max_to_low(
    const struct list_elem *a,
    const struct list_elem *b,
    void *aux
);

/*
 * 定义一个比较线程origin_priority高低的函数，
 * 用于semaphore -> waiters队列“从低到高”的排序
 */
bool origin_priority_cmp_low_to_max(
    const struct list_elem *a,
    const struct list_elem *b,
    void *aux
);

/*
 * 定义一个比较线程origin_priority高低的函数，
 * 用于semaphore -> waiters队列“从高到低”的排序
 */
bool origin_priority_cmp_max_to_low(
    const struct list_elem *a,
    const struct list_elem *b,
    void *aux
);

/* 定义一个用于处理“优先级翻转”的函数 */
void priority_inversion(struct semaphore *sema);

/* Optimization barrier.
 *
 *   The compiler will not reorder operations across an
 *   optimization barrier.  See "Optimization Barriers" in the
 *   reference guide for more information.*/
#define barrier() asm volatile ("" : : : "memory")

#endif /* threads/synch.h */
