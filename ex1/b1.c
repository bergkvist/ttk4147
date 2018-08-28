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

int main() {    
    /* Using times()
    // real    0m5.440s
    // user    0m1.372s
    // sys     0m4.064s

    struct tms t;
    for (int i = 0; i < 10*1000*1000; i++) {
        times(&t);
    }
    // =================== */

    //* Using __rdtsc()
    // real    0m0.081s
    // user    0m0.080s
    // sys     0m0.000s

    for (int i = 0; i < 10*1000*1000; i++) {
        __rdtsc();
    }
    // =================== */


    /* Using clock_gettime()
    // real    0m0.217s
    // user    0m0.216s
    // sys     0m0.000s

    struct timespec t;
    for (int i = 0; i < 10*1000*1000; i++) {
        clock_gettime(CLOCK_MONOTONIC, &t);
    }
    // =================== */
}
