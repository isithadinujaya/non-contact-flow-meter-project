#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ultrasonic.h"

// Simulated function to fill the signal buffer
void acquire_ultrasonic_signal(float *buffer, int length, const char *source) {
    for (int i = 0; i < length; i++) {
        if (strcmp(source, "transmitter") == 0) {
            buffer[i] = sinf(2 * M_PI * 40e3 * i / 1e6);  // 40kHz signal
        } else if (strcmp(source, "receiver") == 0) {
            buffer[i] = sinf(2 * M_PI * 40e3 * (i - 5) / 1e6); // delayed signal
        }
    }
}
