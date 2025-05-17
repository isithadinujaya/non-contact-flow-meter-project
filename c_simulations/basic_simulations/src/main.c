#include <stdio.h>
#include "ultrasonic.h"
#include "correlation.h"

#define SIGNAL_LENGTH 256

int main() {
    float signal1[SIGNAL_LENGTH];
    float signal2[SIGNAL_LENGTH];

    // Simulate or acquire signals
    acquire_ultrasonic_signal(signal1, SIGNAL_LENGTH, "transmitter");
    acquire_ultrasonic_signal(signal2, SIGNAL_LENGTH, "receiver");

    // Compute time delay using cross-correlation
    int delay_index = cross_correlate(signal1, signal2, SIGNAL_LENGTH);

    printf("Estimated delay: %d samples\n", delay_index);

    float flow_velocity = compute_velocity_from_delay(delay_index);
    printf("Estimated flow velocity: %.3f m/s\n", flow_velocity);

    return 0;
}
