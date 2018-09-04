#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

long int global_number = 0;
sem_t mutex;

void* fn(void* args) {
    long int local_number = 0;
    
    for (long int i = 0; i < 50*1000*1000; i++) {
        sem_wait(&mutex);
        global_number++;
        sem_post(&mutex);
        local_number++;
    }
    
    printf("result: [local %ld] [global %ld]\n", local_number, global_number);
    return NULL;
}

int main() {
    
    pthread_t thread_handle_1;
    pthread_t thread_handle_2;

    sem_init(&mutex, 0, 1);
    pthread_create(&thread_handle_1, NULL, fn, NULL);
    pthread_create(&thread_handle_2, NULL, fn, NULL);
    pthread_join(thread_handle_1, NULL);
    pthread_join(thread_handle_2, NULL);
    sem_destroy(&mutex);

    printf("global: %ld\n", global_number);
}

/* ab.c
real    0m0.189s
user    0m0.368s
sys     0m0.000s
*/

/* c.c (mutex wait/post outside for loop)
result: [local 50000000] [global 50000000]
result: [local 50000000] [global 100000000]
global: 100000000

real    0m0.220s
user    0m0.216s
sys     0m0.000s
*/

/* c.c (mutex wait/post inside for loop)
result: [local 50000000] [global 89811632]
result: [local 50000000] [global 100000000]
global: 100000000

real    0m14.932s
user    0m15.380s
sys     0m14.236s
*/