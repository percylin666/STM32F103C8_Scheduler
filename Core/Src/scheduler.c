#include "scheduler.h"

#include "usart.h"

// 内存池和空闲链表
static TaskNode task_pool[TASK_POOL_SIZE];
static TaskNode *free_list = NULL;

// 初始化内存池
void task_pool_init(void) {
    for (int i = 0; i < TASK_POOL_SIZE - 1; ++i) {
        task_pool[i].next = &task_pool[i + 1];
    }
    task_pool[TASK_POOL_SIZE - 1].next = NULL;
    free_list = &task_pool[0];
}

// 从池中分配节点
TaskNode *alloc_task_node(void) {
    if (!free_list) return NULL;
    TaskNode *n = free_list;
    free_list = free_list->next;
    n->next = NULL;
    return n;
}

// 释放节点回池
void free_task_node(TaskNode *n) {
    n->next = free_list;
    free_list = n;
}

// 注册任务（仅在初始化期使用）
int register_task(TaskFunction func, uint32_t interval_ms, TaskNode **reg_task_list_head) {
    if (!reg_task_list_head || !func) return -2;

    TaskNode *node = alloc_task_node();
    if (!node) return -1;

    node->func = func;
    node->interval_ms = interval_ms;
    node->countdown = interval_ms;
    node->next = *reg_task_list_head;
    *reg_task_list_head = node;
    return 0;
}

// 调度器 tick（每 1ms 或指定周期调用）
void scheduler_tick(TaskNode *reg_task_list_head, TaskNode **rdy_task_list_head) {

    TaskNode *cur = reg_task_list_head;
    while (cur) {
        if (cur->countdown > 0) {
            cur->countdown -= 1;
            if (cur->countdown == 0) {
                TaskNode *ready = alloc_task_node();
                if (ready) {
                    ready->func = cur->func;
                    ready->next = *rdy_task_list_head;
                    *rdy_task_list_head = ready;
                }
                cur->countdown = cur->interval_ms;
            }
        }
        cur = cur->next;
    }
}

// 主循环中调用，执行 ready 表中的任务
void scheduler_run(TaskNode **rdy_task_list_head) {
    if (!rdy_task_list_head || !*rdy_task_list_head) return;

    TaskNode *cur = *rdy_task_list_head;
    *rdy_task_list_head = NULL;

    while (cur) {
        TaskNode *next = cur->next;
        if (cur->func) cur->func();
        free_task_node(cur);
        cur = next;
    }
}



// // 示例初始化代码（在 main 或 setup 中调用）
// void scheduler_init(void) {
//     task_pool_init();
//     register_task(task_led_toggle, 100);      // 每 100ms 执行一次
//     register_task(task_printf, 500);     // 每 500ms 执行一次
// }