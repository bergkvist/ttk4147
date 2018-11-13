#pragma once

/* -------------
This file contains global constants that can be accessed anywhere this header is imported!
------------- */

// Communication (only used in communication.c)
#define SERVER_IP "192.168.0.1"
#define SERVER_PORT 9999

// Controller constants (only used in controller.c)
#define K_P 2e1  // [1]
#define K_I 1e3  // [s^-1]
#define K_D 1e-2 // [s]

// The controller period in milliseconds. (1 ms will cause some weird instability)
#define PERIOD_MS 2
