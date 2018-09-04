#include <pthread.h>
#include <stdio.h>

#define FILOSOPHER_COUNT       5
#define TIME_IT_TAKES_TO_EAT   0.5 // seconds 

pthread_mutex_t fork[FILOSOPHER_COUNT];

void* philosopher_fn(void* i) {
    
    int left = (int) i;
    int right = ((int) i + 1) % FILOSOPHER_COUNT;

    while (1) {
        printf("Starting: %d\n", (int) i);
        pthread_mutex_lock(&fork[left]);
        pthread_mutex_lock(&fork[right]);

        printf("Eating: %d\n", (int) i);
        sleep(TIME_IT_TAKES_TO_EAT);

        pthread_mutex_unlock(&fork[right]);
        pthread_mutex_unlock(&fork[left]);
        printf("Finished: %d\n", (int) i);
    }
    
    return NULL;
}


int main() {
    pthread_t philosopher_handle[FILOSOPHER_COUNT];
    
    for (int i = 0; i < FILOSOPHER_COUNT; i++)
        pthread_mutex_init(&fork[i], NULL);
    
    for (int i = 0; i < FILOSOPHER_COUNT; i++) 
        pthread_create(&philosopher_handle[i], NULL, philosopher_fn, (void *) i);
    
    for (int i = 0; i < FILOSOPHER_COUNT; i++)
        pthread_join(philosopher_handle[i], NULL);

    for (int i = 0; i < FILOSOPHER_COUNT; i++)
        pthread_mutex_destroy(&fork[i]);
        
    printf("All done!\n");
}