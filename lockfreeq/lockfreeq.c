/**
 * @file    Lock Free Queue.c
 * @brief   Development for a fast inter-thread communication in a single process
 * @TODO	Need for inter-process one for distributed clustering system 
 *
 * @authors	Seong-Joong Kim (sungjungk@gmail.com)
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "atomic.h"
#include "lock_free_q.h"


lock_free_queue_t queue;


void
init_lock_free_queue(void)
{
	memset(&queue, '\0', sizeof(lock_free_queue_t));
	queue.reader_cursor.pos = -1;
}


static void
writer_waiting_next_entry(cursor_t* const cursor)
{
	const cursor_t new_pos = {1 + atomic_fetch_and_add(&queue.writer_cursor.pos, 1)};
	cursor_t slowest_reader;

	cursor->pos = new_pos.pos;

	while (true) {
		slowest_reader.pos = atomic_load(&queue.reader_cursor.pos);

		if (UNLIKELY(-1 == slowest_reader.pos)) 
			slowest_reader.pos = new_pos.pos - ((QUEUE_SIZE - 1) & new_pos.pos);

		atomic_store(&queue.reader_cursor.pos, slowest_reader.pos);
		
		if (LIKELY((cursor->pos - slowest_reader.pos) <= (QUEUE_SIZE - 1))) 
			return ;

		nanosleep(&timeout.timeout, NULL);
	}
}

static entry_t*
writer_acquire_entry(cursor_t* const cursor)
{
	return &queue.buffer[(QUEUE_SIZE - 1) & cursor->pos];
}

static void 
writer_push_entry(const cursor_t* __restrict__ const cursor)
{
	const atomic_t tmp = cursor->pos - 1;
	
	while (atomic_load(&queue.reader_upper_cursor.pos) != tmp) 
		nanosleep (&timeout.timeout, NULL);

	atomic_fetch_and_add(&queue.reader_upper_cursor.pos, 1);
}




/*
 *	defines enqueue & dequeue functions
 */

void
enqueue(uint64_t data)
{
	cursor_t cursor;
	entry_t *entry;

	writer_waiting_next_entry(&cursor);
	entry = writer_acquire_entry(&cursor);
	entry->data = data;
	writer_push_entry(&cursor);
}

const entry_t*
dequeue(void)
{
	const entry_t *entry;
	
	while (atomic_load(&queue.reader_cursor.pos) == -1 || 
		   atomic_load(&queue.reader_cursor.pos) > atomic_load(&queue.reader_upper_cursor.pos)){
		nanosleep(&timeout.timeout, NULL);
	}

	entry = &queue.buffer[(QUEUE_SIZE - 1) & atomic_fetch_and_add(&queue.reader_cursor.pos, 1)];

	return entry;
}

/* end of lockfreeq.c */
