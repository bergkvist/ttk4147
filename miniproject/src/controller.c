#include "controller.h"
#include "config.h"

#define PERIOD_S (PERIOD_MS * 1e-3)

static volatile float previous_error = 0;
static volatile float integral = 0;

float controller_pid (float error) {
    float derivative;
    float control_signal;

    integral += error * PERIOD_S;
    derivative = (error - previous_error) / PERIOD_S;
    control_signal = (K_P * error) + (K_I * integral) + (K_D * derivative);

    previous_error = error;
    return control_signal;
}
