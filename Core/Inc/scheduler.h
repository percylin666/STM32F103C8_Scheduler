#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include <stdint.h>
#include <stddef.h>
#include "main.h"

#define TASK_POOL_SIZE 16

typedef void (*TaskFunction)(void);

typedef struct TaskNode {
    TaskFunction func;
    uint32_t interval_ms;
    uint32_t countdown;
    struct TaskNode *next;
} TaskNode;


void task_pool_init(void);
TaskNode *alloc_task_node(void);
void free_task_node(TaskNode *n);
int register_task(TaskFunction func, uint32_t interval_ms, TaskNode **rdy_taks_list_head);
void scheduler_tick(TaskNode *reg_taks_list_head, TaskNode **rdy_task_list_head);
void scheduler_run(TaskNode **rdy_task_list_head);

#endif