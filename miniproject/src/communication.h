#pragma once

/** -------------------------------------------------------
 *  y: The observed state of the system
 *  u: The actuation forces applied to the system
 * ----------------------------------------------------- */

/**
 * Run this before starting the threads
 */
void communication_init(void);

/**
 * Run this after stopping the threads
 */
void communication_cleanup(void);

/**
 * These are pthread functions that should be started as threads in
 * order for the other functions in this module to work.
 */
void* communication_requester_thread(void* _);
void* communication_receiver_thread(void* _);

/**
 * Halt progress (non-busy waiting) until 
 * a new y-value is received.
 */
float communication_await_y(void);

/**
 * Send a new u-value to the server.
 */ 
void communication_send_u(float u);
