#include <assert.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>
#include <x86intrin.h>

void busy_wait(long int time_seconds) {
    struct tms t;
    clock_t t0 = times(&t);
    while(times(&t) - t0 <= time_seconds * 100)
        ;
}

int main() {
    busy_wait(1);
}