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
    /*
    int ns_max = 1500; 
    int histogram[ns_max]; 
    memset(histogram, 0, sizeof(int)*ns_max); 
    for(int i = 0; i < 10*1000*1000; i++){ 
        long int t1 = __rdtsc();
        sched_yield();
        long int t2 = __rdtsc();
        int ns = (t2 - t1);
        if (ns >= 0 && ns < ns_max) { 
            histogram[ns]++; 
        } 
    } 
    for(int i = 0; i < ns_max; i++){ 
        printf("%d\n", histogram[i]); 
    }
    //*/

    /*
    struct tms t1;
    struct tms t2;
    int ns_max = 2000; 
    int histogram[ns_max]; 
    memset(histogram, 0, sizeof(int)*ns_max); 
    for(int i = 0; i < 10*1000*1000; i++){ 
        times(&t1);
        sched_yield();
        times(&t2);
        int ns = (t2.tms_stime + t2.tms_utime - t1.tms_stime - t1.tms_utime) * 10 * 1000 * 1000;
        if (ns >= 0 && ns < ns_max) { 
            histogram[ns]++; 
        } 
    } 
    for(int i = 0; i < ns_max; i++){ 
        printf("%d\n", histogram[i]); 
    }
    //*/

    /*
    struct timespec t1;
    struct timespec t2;
    int ns_max = 1500; 
    int histogram[ns_max]; 
    memset(histogram, 0, sizeof(int)*ns_max); 
    for(int i = 0; i < 10*1000*1000; i++){ 
        clock_gettime(CLOCK_MONOTONIC, &t1);
        sched_yield();
        clock_gettime(CLOCK_MONOTONIC, &t2);
        int ns = (t2.tv_nsec - t1.tv_nsec);
        if (ns >= 0 && ns < ns_max) { 
            histogram[ns]++; 
        } 
    } 
    for(int i = 0; i < ns_max; i++){ 
        printf("%d\n", histogram[i]); 
    }
    //*/
}
