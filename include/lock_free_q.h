/**
 * @file    lock_free_q.h
 * @brief   TODO
 *
 * @authors Seong-Joong Kim (sungjungk@gmail.com)
 */

#ifndef _LOCKFREEQ_H
#define _LOCKFREEQ_H

#include <stdint.h>				/* for some types  	  				*/
#include <time.h>				/* for nanosleep related functions 	*/

#include "atomic.h"


#define QUEUE_SIZE				4096	/* Size of the Queue */

#ifndef PAGE_SIZE
#define PAGE_SIZE	4096
#endif

#ifndef CACHE_LINE_SIZE
#define CACHE_LINE_SIZE	64	
#endif

#ifndef BUFFER_SIZE
#define BUFFER_SIZE	8192
#endif

#ifdef LIKELY
#undef LIKELY
#endif
#define LIKELY(expr__) (__builtin_expect(((expr__) ? 1 : 0), 1))

#ifdef UNLIKELY
#undef UNLIKELY
#endif
#define UNLIKELY(expr__) (__builtin_expect(((expr__) ? 1 : 0), 0))

#define tb_bool_t uchar                                                         
#ifndef true                                                                    
#   define true  1                                                              
# 	define false 0                                                              
#endif  



typedef struct yield_s yield_t;
struct yield_s {
	struct timespec timeout;
} __attribute__((aligned(CACHE_LINE_SIZE)));

const yield_t timeout = {{0, 1}};



typedef struct count_s count_t;
struct count_s {
	atomic_t cnt;
} __attribute__((aligned(CACHE_LINE_SIZE)));

typedef struct cursor_s cursor_t;
struct cursor_s {
	atomic_t pos;
} __attribute__((aligned(CACHE_LINE_SIZE)));

typedef struct entry_s entry_t;
struct entry_s {
	uint64_t data;
} __attribute__((aligned(CACHE_LINE_SIZE)));

typedef struct lock_free_queue_s lock_free_queue_t;
struct lock_free_queue_s {
	/* for publishers */
	cursor_t writer_cursor;

	/* for subscribers */
	cursor_t reader_cursor;
	cursor_t reader_upper_cursor;

	entry_t buffer[QUEUE_SIZE];
} __attribute__((aligned(PAGE_SIZE)));


extern lock_free_queue_t queue;


/* Function declaration */
void init_lock_free_queue(void);

void enqueue(uint64_t data);
const entry_t* dequeue(void);

#endif
