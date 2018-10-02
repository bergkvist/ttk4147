
// #define RT_TASK RT_TASK
// #define RT_TASK_INFO RT_TASK_INFO 
// #define rt_task_create rt_task_create /* ( RT_TASK *task, const char *name, int stksize, int prio, int mode ) */
// #define rt_task_start rt_task_start /* ( RT_TASK *task, void(*entry)(void *cookie), void *cookie ) */
// #define rt_task_set_periodic rt_task_set_periodic /* ( RT_TASK *task, RTIME idate, RTIME period ) */
// #define rt_task_wait_period rt_task_wait_period /* ( unsigned long *overruns_r ) */

// #define mlockall mlockall
// #define MCL_CURRENT MCL_CURRENT
// #define MCL_FUTURE MCL_FUTURE

#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <rtdk.h>
#include "io.h"

// function to be executed by task
void helloWorld(void *arg) {
    printf("Hello World!\n");
}

int main() {
    RT_TASK hello_task;
    rt_task_create(&hello_task, "hello_task", 0, 50, 0);
    rt_task_start(&hello_task, &helloWorld, 0);
}
