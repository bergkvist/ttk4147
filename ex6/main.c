#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <rtdk.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#include "io.h"

#define DISTURBANCE_COUNT 5
#define TEST_THREAD_COUNT 3

int set_cpu(int cpu_number) {
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_number, &cpu);
    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

void fn(void* args) {
    unsigned long duration = 50000000000;  // 50 Second timeout
    unsigned long endTime  = rt_timer_read() + duration;
    int *ptr = (int*)args;
    int pin = *ptr;
    rt_printf("%d\n", pin);
    for (;;) {
        if (io_read(pin) == 0) {
            io_write(pin, 0);
            //usleep(50);  // Fix this
            io_write(pin, 1);
        }
        if (rt_timer_read() > endTime) {
            rt_printf("Time expired\n");
            rt_task_delete(NULL);
        }
        if (rt_task_yield()) {
            rt_printf("Task failed to yield\n");
            rt_task_delete(NULL);
        }
        rt_task_wait_period(NULL);
    }
    return NULL;
}

void periodic_fn(void* args) {
    unsigned long period = 1000000;
    int *ptr = (int*)args;
    int pin = *ptr;
    rt_printf("%d\n", pin);
    rt_task_set_periodic(NULL, TM_NOW, period);
    for (;;) {
        if (io_read(pin) == 0) {
            io_write(pin, 0);
            io_write(pin, 1);
        }
        rt_task_wait_period(NULL);
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
    rt_print_auto_init(1);
    mlockall(MCL_CURRENT|MCL_FUTURE);
    io_init();

    int pins[] = {1,2,3};

    RT_TASK task[TEST_THREAD_COUNT];
    pthread_t thread_disturbance_handle[DISTURBANCE_COUNT];

    for (int i = 0; i < DISTURBANCE_COUNT; i++)
        pthread_create(&thread_disturbance_handle[i], NULL, disturbance_fn, NULL);

    for (int pin = 0; pin < TEST_THREAD_COUNT; pin++) {
        rt_task_create(&task[pin], NULL, 0, 50, T_CPU(1));
        rt_task_start(&task[pin], &periodic_fn, &pins[pin]);
    }   

    while(1){
        sleep(-1);
    }

    printf("DONE");
}
