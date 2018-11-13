#pragma once

/** -------------------------------------------------------
 *  y: The observed state of the system
 *  u: The control signal applied to the system
 * ----------------------------------------------------- */

/**
 * Run this before using the other communication functions.
 * 
 * Will connect to the server and start 2 pthreads:
 *  requester -> send "GET" commands periodically (based on period in config.h)
 *  receiver  -> receive "GET_ACK" and "SIGNAL" messages.
 */
void communication_init(void);

/**
 * Run this after you are finished using the communication functions.
 * 
 * Will stop the 2 pthreads that communication_init created,
 * and disconnect from the server.
 */
void communication_cleanup(void);

/**
 * Halt progress (non-busy waiting) until a new y-value is received.
 */
float communication_await_y(void);

/**
 * Send a new u-value (control signal) to the server.
 */ 
void communication_send_u(float u);
