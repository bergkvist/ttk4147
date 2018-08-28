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
#include "./array.h"

int main() {
    Array a = array_new(7);
    array_insertBack(&a, 1);
    array_insertBack(&a, 2);
    array_insertBack(&a, 3);
    array_insertBack(&a, 4);
    array_insertBack(&a, 5);
    array_insertBack(&a, 6);
    array_insertBack(&a, 7);
    array_print(a);
}