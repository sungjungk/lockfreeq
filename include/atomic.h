/**
 * @file    atomic.h
 * @brief   Implements of some atomic operations
 *
 * @authors Seong-Joong Kim (sungjungk@gmail.com)
 */

#ifndef _ATOMIC_H
#define _ATOMIC_H

#include <stdint.h>

typedef int64_t atomic_t;


/*
 *	Atomic Load operation which consider x86 64-bit memory model
 */
static inline atomic_t 
atomic_load(volatile atomic_t *ptr)
{
	return *ptr;
}

static inline atomic_t 
atomic_load_acquire(volatile atomic_t *ptr)
{
	atomic_t value = *ptr;
	asm __volatile__("mfence" ::: "memory");
	return value;
}

static inline atomic_t 
atomic_load_release(volatile atomic_t *ptr)
{
	asm __volatile__("mfence" ::: "memory");
	return *ptr;
}


static inline void
atomic_store(volatile atomic_t *ptr, atomic_t value)
{
	*ptr = value;
}

static inline void
atomic_store_acquire(volatile atomic_t *ptr, atomic_t value)
{
	*ptr = value;
	asm __volatile__("mfence" ::: "memory");
}

static inline void
atomic_store_release(volatile atomic_t *ptr, atomic_t value)
{
	asm __volatile__("mfence" ::: "memory");
	*ptr = value;
}



static inline atomic_t
atomic_fetch_and_add(volatile atomic_t *ptr, atomic_t value)
{
	asm __volatile__(
		"lock; xaddl %%eax, %2;"
	 	:"=a" (value)
		:"a" (value), "m" (*ptr)
		:"cc", "memory"
	);
	return value;
}


static inline void
atomic_add(volatile atomic_t *ptr, atomic_t value)
{
    asm __volatile__(
        "lock; addq %1,%0"
        :"=m" (*ptr)
        :"ir" (value), "m" (*ptr)
    );
}


static inline void
atomic_sub(volatile atomic_t *ptr, atomic_t value)
{
    asm __volatile__(
        "lock; subq %1,%0"
        :"=m" (*ptr)
        :"ir" (value), "m" (*ptr)
    );
}

static inline void
atomic_inc(volatile atomic_t *ptr)
{
    asm __volatile__(
        "lock; incq %0"
        :"=m" (*ptr)
        :"m" (*ptr)
    );
}

static inline void
atomic_dec(volatile atomic_t *ptr)
{
    asm __volatile__(
        "lock; decq %0"
        :"=m" (*ptr)
        :"m" (*ptr)
    );
}


static inline atomic_t
atomic_cas(volatile atomic_t *ptr, atomic_t new_value, atomic_t old_value)
{
    asm __volatile__(
        "lock; cmpxchgq %1,(%3)"
        : "=a" (new_value)
        : "r" (new_value), "a" (old_value), "r" (ptr)
        : "cc", "memory"
    );

    return new_value;
}

#endif
