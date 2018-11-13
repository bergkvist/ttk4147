#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "communication.h"
#include "config.h"
#include "../lib/udp.h"
#include "../lib/time.h"

#define EVER ;;
#define BUFFER_SIZE 32
const struct timespec PERIOD = {.tv_sec = 0, .tv_nsec = (PERIOD_MS * 1e6)};

float y;                    // The latest y-value/observed state of the system
sem_t y_sem;                // Used to indicate/wait for a new y to be available.
pthread_t receiver_handle;
pthread_t requester_handle;
UDPConn* connection;

static void* requester_thread(void* _) {
    struct timespec waketime;
    clock_gettime(CLOCK_REALTIME, &waketime);
    for (EVER) {
        udpconn_send(connection, "GET");
        waketime = timespec_add(waketime, PERIOD);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &waketime, NULL);
    }
    return NULL;
}

static void* receiver_thread(void* _) {
    for (EVER) {
        char receive_buffer[BUFFER_SIZE];
        memset(receive_buffer, 0, sizeof(receive_buffer));
        udpconn_receive(connection, receive_buffer, sizeof(receive_buffer));
        /* ^ Wait for the next message from the server */

        /**
         * WARNING: This implementation is vulnerable to changes in the server.
         * If a new message (server -> client) starting with 'S' or 'G' is added, 
         * this will be a problem.
         */
        const char first_letter = receive_buffer[0];
        if (first_letter == 'S') {
            // This must be a SIGNAL message, so we acknowledge it!
            udpconn_send(connection, "SIGNAL_ACK");

        } else if (first_letter == 'G') {
            // We know this must be a GET_ACK message!
            y = atof(&receive_buffer[8]);
            /**                      ^
             * Offset of y-value in GET_ACK messages:
             * G  E  T  _  A  C  K  :  1  2  3  .  4  5  6
             * 0  1  2  3  4  5  6  7 {8} 9 10 11 12 13 14 
             */
            sem_post(&y_sem); // Notify that a new y is available
        }
    }
    return NULL;
}


void communication_init(void) {
    connection = udpconn_new(SERVER_IP, SERVER_PORT);
    udpconn_send(connection, "START");
    pthread_create(&receiver_handle, NULL, receiver_thread, NULL);
    pthread_create(&requester_handle, NULL, requester_thread, NULL);
}

void communication_cleanup(void) {
    udpconn_send(connection, "STOP");
    pthread_cancel(receiver_handle);
    pthread_cancel(requester_handle);
    udpconn_delete(connection);
}


float communication_await_y() {
    sem_wait(&y_sem);
    return y;
}

void communication_send_u(const float u) {
    char send_buffer[BUFFER_SIZE];
    memset(send_buffer, 0, sizeof(send_buffer));
    snprintf(send_buffer, sizeof(send_buffer), "SET:%f", u);
    udpconn_send(connection, send_buffer);
}
