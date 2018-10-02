
// #define RT_TASK RT_TASK
// #define RT_TASK_INFO RT_TASK_INFO 
// #define rt_task_create rt_task_create /* ( RT_TASK *task, const char *name, int stksize, int prio, int mode ) */
// #define rt_task_start rt_task_start /* ( RT_TASK *task, void(*entry)(void *cookie), void *cookie ) */
// #define rt_task_set_periodic rt_task_set_periodic /* ( RT_TASK *task, RTIME idate, RTIME period ) */
// #define rt_task_wait_period rt_task_wait_period /* ( unsigned long *overruns_r ) */

// #define mlockall mlockall
// #define MCL_CURRENT MCL_CURRENT
// #define MCL_FUTURE MCL_FUTURE

// In order to auto-compile and run on file-changes:
// while true; do ./sleep_until_modified.sh main.c; make && ./a.out; done
#define _GNU_SOURCE
#include <native/task.h>
#include <native/timer.h>
#include <pthread.h>
#include <sched.h>
#include <sys/mman.h>
#include <rtdk.h>
#include "io.h"

#define DISTURBANCE_COUNT 5

// function to be executed by task
void* fn(void* args) {
    int pin = (int) args;
    for(int i = 0;i < 100;i++) {
        if (io_read(pin) == 0) {
            io_write(pin, 0);
            rt_printf("yolo");
            io_write(pin, 1);
        }
    }
    return NULL;
}

void* disturbance_fn(void* args) {
    set_cpu(1);
    for(;;) {
        asm volatile("" ::: "memory");
    }
}


int main() {
    mlockall(MCL_CURRENT|MCL_FUTURE);
    io_init();

    unsigned long duration = 10*1000*1000*1000;  // 10 Second timeout
    unsigned long endTime  = rt_timer_read() + duration;

    //RT_TASK task[3];
    pthread_t thread_disturbance_handle[DISTURBANCE_COUNT];

    /*for (int pin = 1; pin <= 3; pin++) {
        rt_task_create(&task[pin], "task", 0, 50, 0);
        rt_task_start(&task[pin], &fn, pin);
    }*/

    /*while (1) {
        if (rt_timer_read() > endTime) {
            rt_printf("Time expired\n");
            rt_task_delete(NULL);
        }
        if (rt_task_yield()) {
            rt_printf("Task failed to yield\n");
            rt_task_delete(NULL);
        }
    }*/
    printf("DONE!");
}
