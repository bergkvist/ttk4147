#include <stdio.h>
#include <pthread.h>
#include "communication.h"
#include "controller.h"
#include "config.h"

int main() {
    const float y_ref = 0;

    pthread_t comm_receiver_handle;
    pthread_t comm_requester_handle;

    communication_init();
    pthread_create(&comm_receiver_handle, NULL, communication_receiver_thread, NULL);
    pthread_create(&comm_requester_handle, NULL, communication_requester_thread, NULL);

    unsigned int i;
    for (i = 0; i < 2 / PERIOD_S; i++) {
        /* Run for ~2 seconds */
        const float y = communication_await_y();
        const float u = controller_pid(y_ref - y);
        communication_send_u(u);
    }
    
    pthread_cancel(comm_receiver_handle);
    pthread_cancel(comm_requester_handle);
    communication_cleanup();

    printf("Finished successfully!\n");
}
