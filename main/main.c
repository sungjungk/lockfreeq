/**
 * @file    main.c
 * @brief   TODO
 *
 * @authors	Seong-Joong Kim (sungjungk@gmail.com)
 */

#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>				/* for sleep function */
#include <assert.h>

#include "lock_free_q.h"
#include "common.h"

#define ENTRIES_TO_GENERATE  100000


static void*
init_reader_thread(void *args)
{
	const entry_t *entry;

	while (1) {
		entry = dequeue(); 

		if (!entry->data)
			continue;

		if (entry->data == -1)
			break;
	}

	printf("Reader done\n");

	return 0;
}


int 
create_readers(void)
{
	pthread_t tid;
	pthread_attr_t attr;
	thread_args_t *args;
	int i;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setstacksize(&attr, THREAD_STACK_SIZE);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

#define READER_NUM				16
	for (i = 0; i < READER_NUM; i++) {
		args = malloc(sizeof(thread_args_t));
		args->tid = i;
		snprintf(args->name, THREAD_NAME_LEN, "READER_%d", i);
		if (pthread_create(&tid, &attr, init_reader_thread, args))
			assert(0);
	}

	return 0;
}

static atomic_t send_data = 1;
static void*
init_writer_thread(void *args)
{
	uint64_t reps = ENTRIES_TO_GENERATE;

	while (reps--) {
		enqueue(atomic_fetch_and_add(&send_data, 1));
	}

	printf("Write done\n");

	return NULL;
}

int
create_writers(void)
{
	pthread_t tid;
	pthread_attr_t attr;
	thread_args_t *args;
	int i;

	pthread_attr_init(&attr);
	//pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setstacksize(&attr, THREAD_STACK_SIZE);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

#define WRITER_NUM				16
	for (i = 0; i < WRITER_NUM; i++) {
		args = malloc(sizeof(*args));
		args->tid = i + READER_NUM;
		snprintf(args->name, THREAD_NAME_LEN, "WRITER_%d", i);
		if (pthread_create(&tid, &attr, init_writer_thread, args))
			assert(0);
	}

	return 0;
}


static void*
init_monitor_thread(void *args)
{
	/* Nothing todo */

	create_readers();

	create_writers();

	while(true);

	return 0;
}

static pthread_t
create_monitor(void)
{
	pthread_t tid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setstacksize(&attr, THREAD_STACK_SIZE);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	if (pthread_create(&tid, &attr, init_monitor_thread, NULL))
		assert(0);

	return tid;
}

int
main(int argc, char *argv[])
{
	pthread_t mthr;

	init_lock_free_queue();

	mthr = create_monitor();
	
	pthread_join(mthr, NULL);

	printf("Lock-Free Queue test done\n\n");

	return 0;
}

/* end of main.c */
