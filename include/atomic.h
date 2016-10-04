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


static inline long
atomic_load(volatile atomic_t *vp)
{
	return *vp;
}

static inline long
atomic_load_seq_cst(volatile atomic_t *vp)
{
	asm __volatile__("mfence" ::: "memory");
	return *vp;
}


static inline void
atomic_store(volatile atomic_t *vp, long nv)
{
	*vp = nv;
}

static inline void
atomic_store_seq_cst(volatile atomic_t *vp, long nv)
{
	asm __volatile__("mfence" ::: "memory");
	*vp = nv;
	asm __volatile__("mfence" ::: "memory");
}

static inline long
atomic_fetch_and_add(volatile atomic_t *vp, long nv)
{
	asm __volatile__(
		"lock; xaddl %%eax, %2;"
	 	:"=a" (nv)
		:"a" (nv), "m" (*vp)
		:"cc", "memory"
	);
	return nv;
}


static inline void
atomic_add(volatile atomic_t *vp, long v)
{
    asm __volatile__(
        "lock; addq %1,%0"
        :"=m" (*vp)
        :"ir" (v), "m" (*vp)
    );
}


static inline void
atomic_sub(volatile atomic_t *vp, long v)
{
    asm __volatile__(
        "lock; subq %1,%0"
        :"=m" (*vp)
        :"ir" (v), "m" (*vp)
    );
}

static inline void
atomic_inc(volatile atomic_t *vp)
{
    asm __volatile__(
        "lock; incq %0"
        :"=m" (*vp)
        :"m" (*vp)
    );
}

static inline void
atomic_dec(volatile atomic_t *vp)
{
    asm __volatile__(
        "lock; decq %0"
        :"=m" (*vp)
        :"m" (*vp)
    );
}


static inline long
atomic_cas(volatile atomic_t *vp, long nv, long ov)
{
    asm __volatile__(
        "lock; cmpxchgq %1,(%3)"
        : "=a" (nv)
        : "r" (nv), "a" (ov), "r" (vp)
        : "cc", "memory"
    );

    return nv;
}

#endif
