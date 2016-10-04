/**
 * @file    common.h
 * @brief   TODO
 *
 * @authors Seong-Joong Kim (sungjungk@gmail.com)
 */


#ifndef _COMMON_H
#define _COMMON_H

#define THREAD_NAME_LEN			16

#define THREAD_STACK_SIZE		512 * 1024

typedef struct thread_args_s thread_args_t;
struct thread_args_s {
	int tid;
	char name[THREAD_NAME_LEN];
};

#endif
