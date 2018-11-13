#include <stdio.h>
#include <pthread.h>
#include "communication.h"
#include "controller.h"
#include "config.h"

#define SECOND (1000 / PERIOD_MS)

int main() {

    communication_init();

    unsigned int t;
    for (t = 0; t < 2 * SECOND; t++) {
        // Reference starts at 1, and changes to 0 after 1 second
        const float y_reference = (t < 1 * SECOND);  // I know, kind of hacky
        const float y = communication_await_y();
        const float u = controller_pid(y_reference - y);
        communication_send_u(u);
    }

    communication_cleanup();

    printf("Finished successfully! Exiting...\n");
    return 0;
}
