#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#include "io.h"

#define DISTURBANCE_COUNT 5

struct timespec timespec_normalized(time_t sec, long nsec){
    while(nsec >= 1000000000){
        nsec -= 1000000000;
        ++sec;
    }
    while(nsec < 0){
        nsec += 1000000000;
        --sec;
    }
    return (struct timespec){sec, nsec};
}

struct timespec timespec_add(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec + rhs.tv_sec, lhs.tv_nsec + rhs.tv_nsec);
}

int set_cpu(int cpu_number) {
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(cpu_number, &cpu);

    return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}


void* fn(void* args) {
    struct timespec waketime;
    struct timespec period = {.tv_sec = 0, .tv_nsec = 1 * 1000 * 1000};
    clock_gettime(CLOCK_REALTIME, &waketime);
    set_cpu(1);

    int pin = (int) args;
    for(;;) {
        if (io_read(pin) == 0) {
            io_write(pin, 0);
            usleep(50);
            io_write(pin, 1);
        }
        waketime = timespec_add(waketime, period);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
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
    io_init();
    
    pthread_t thread_handle[3];
    pthread_t thread_disturbance_handle[DISTURBANCE_COUNT];

    /*for (int i = 0; i < DISTURBANCE_COUNT; i++)
        pthread_create(&thread_disturbance_handle[i], NULL, disturbance_fn, NULL);*/

    for (int pin = 1; pin <= 3; pin++)
        pthread_create(&thread_handle[pin-1], NULL, fn, (void *) pin);
    
    for (int pin = 1; pin <= 3; pin++)
        pthread_join(thread_handle[pin-1], NULL);
    
    /*for (int i = 0; i < DISTURBANCE_COUNT; i++)
        pthread_join(thread_disturbance_handle[i], NULL);*/

    printf("DONE");
}
