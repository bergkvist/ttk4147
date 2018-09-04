#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define TRUE                   1
#define FILOSOPHER_COUNT       5
#define TIME_IT_TAKES_TO_EAT   1 // seconds


pthread_mutex_t philosopher_fork[FILOSOPHER_COUNT];
pthread_mutex_t waiter;

void* philosopher_fn(void* i) {
    
    int left = (int) i;
    int right = ((int) i + 1) % FILOSOPHER_COUNT;

    while (TRUE) {
        printf("Starting: %d\n", (int) i);
        pthread_mutex_lock(&waiter);
        pthread_mutex_lock(&philosopher_fork[left]);
        pthread_mutex_lock(&philosopher_fork[right]);
        pthread_mutex_unlock(&waiter);

        printf("Eating: %d\n", (int) i);
        sleep(TIME_IT_TAKES_TO_EAT);

        pthread_mutex_unlock(&philosopher_fork[right]);
        pthread_mutex_unlock(&philosopher_fork[left]);
        printf("Finished: %d\n", (int) i);
    }
    
    return NULL;
}


int main() {
    pthread_t philosopher_handle[FILOSOPHER_COUNT];
    
    pthread_mutex_init(&waiter, NULL);
    for (int i = 0; i < FILOSOPHER_COUNT; i++)
        pthread_mutex_init(&philosopher_fork[i], NULL);
    
    for (int i = 0; i < FILOSOPHER_COUNT; i++) 
        pthread_create(&philosopher_handle[i], NULL, philosopher_fn, (void *) i);
    
    for (int i = 0; i < FILOSOPHER_COUNT; i++)
        pthread_join(philosopher_handle[i], NULL);

    pthread_mutex_destroy(&waiter);
    for (int i = 0; i < FILOSOPHER_COUNT; i++)
        pthread_mutex_destroy(&philosopher_fork[i]);
        
    printf("All done!\n");
}