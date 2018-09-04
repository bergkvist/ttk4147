#include <stdio.h>
#include <pthread.h>

long int global_number = 0;

void* fn(void* args) {
    long int local_number = 0;
    for (long int i = 0; i < 50*1000*1000; i++) {
        global_number++;
        local_number++;
    }
    printf("result: [local %ld] [global %ld]\n", local_number, global_number);
    return NULL;
}

int main() {
    pthread_t thread_handle_1;
    pthread_t thread_handle_2;

    pthread_create(&thread_handle_1, NULL, fn, NULL);
    pthread_create(&thread_handle_2, NULL, fn, NULL);

    pthread_join(thread_handle_1, NULL);
    pthread_join(thread_handle_2, NULL);

    printf("global: %ld\n", global_number);
}

/*
result: [local 50000000] [global 50028489]
result: [local 50000000] [global 50159836]
global: 50159836
*/

/* When using -g -fsanitize=thread to compile:
==================
WARNING: ThreadSanitizer: data race (pid=5991)
  Write of size 8 at 0x0000014ace38 by thread T2:
    #0 fn /home/student/bergkvist/ttk4147/ex2/a1.c:9 (a.out+0x0000004a1f7f)

  Previous write of size 8 at 0x0000014ace38 by thread T1:
    #0 fn /home/student/bergkvist/ttk4147/ex2/a1.c:9 (a.out+0x0000004a1f7f)

  Location is global '<null>' at 0x000000000000 (a.out+0x0000014ace38)

  Thread T2 (tid=5994, running) created by main thread at:
    #0 pthread_create <null> (a.out+0x000000422236)
    #1 main /home/student/bergkvist/ttk4147/ex2/a1.c:21 (a.out+0x0000004a2039)

  Thread T1 (tid=5993, running) created by main thread at:
    #0 pthread_create <null> (a.out+0x000000422236)
    #1 main /home/student/bergkvist/ttk4147/ex2/a1.c:20 (a.out+0x0000004a201a)

SUMMARY: ThreadSanitizer: data race /home/student/bergkvist/ttk4147/ex2/a1.c:9 in fn
==================
*/