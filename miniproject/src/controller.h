#pragma once

/**
 * Given an error, compute the actuation force u that will
 * make this error converge to 0 over time.
 */
float controller_pid(float error);
