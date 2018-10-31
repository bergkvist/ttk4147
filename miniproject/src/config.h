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

// The period in milliseconds. Used indirectly by both controller.c and communication.c:
#define PERIOD_MS 3

// These are nice to have:
#define PERIOD_S (PERIOD_MS * 1e-3)
#define PERIOD_US (PERIOD_MS * 1e3)
