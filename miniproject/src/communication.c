#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "communication.h"
#include "config.h"
#include "../lib/udp.h"

#define BUFFER_SIZE 32

static float y;
static sem_t y_sem;
static UDPConn* connection;

static void send_message(const char *message);

// ---------------- PUBLIC -------------------

void communication_init(void) {
    connection = udpconn_new(SERVER_IP, SERVER_PORT);
    send_message("START");
}

void communication_cleanup(void) {
    send_message("STOP");
    udpconn_delete(connection);
}

void* communication_requester_thread(void* _) {
    for (;;) {
        send_message("GET");
        usleep(PERIOD_US);
    }
    return NULL;
}

void* communication_receiver_thread(void* _) {
    for (;;) {
        char recv_buffer[BUFFER_SIZE];
        memset(recv_buffer, 0, sizeof(recv_buffer));
        udpconn_receive(connection, recv_buffer, sizeof(recv_buffer));
        /* ^ Wait for the next message from the server */

        /**
         * WARNING: This implementation is vulnerable to changes in the server.
         * If a new message (server -> client) starting with 'S' or 'G' is added, 
         * this will be a problem.
         */
        const char first_letter = recv_buffer[0];
        if (first_letter == 'S') {
            // This must be a SIGNAL message, so we acknowledge it!
            send_message("SIGNAL_ACK");

        } else if (first_letter == 'G') {
            // We know this must be a GET_ACK message!
            y = atof(&recv_buffer[8]);
            /*------------------- ^ -----------------------+
             | Offset of y-value in GET_ACK messages:      |
             | G  E  T  _  A  C  K  :  1  2  3  .  4  5  6 |
             | 0  1  2  3  4  5  6  7 {8} 9 10 11 12 13 14 |
             +--------------------------------------------*/
            sem_post(&y_sem); // Notify that a new y is available
        }
    }
    return NULL;
}

float communication_await_y() {
    sem_wait(&y_sem);
    return y;
}

void communication_send_u(const float u) {
    char send_buffer[BUFFER_SIZE];
    memset(send_buffer, 0, sizeof(send_buffer));
    snprintf(send_buffer, BUFFER_SIZE, "SET:%f", u);
    udpconn_send(connection, send_buffer);
}

// ---------------- INTERNALS -------------------

static void send_message(const char *message) {
    char send_buffer[BUFFER_SIZE];
    memset(send_buffer, 0, sizeof(send_buffer));
    sprintf(send_buffer, (char*)message);
    udpconn_send(connection, send_buffer);
}
