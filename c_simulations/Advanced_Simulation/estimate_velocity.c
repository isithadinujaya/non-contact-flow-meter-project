// estimate_velocity.c

#include <stdio.h>
#include <stdlib.h>

#define FS 1000000           // Sampling rate (1 MHz)
#define SENSOR_DISTANCE 0.1  // 10 cm distance between sensors
#define SPEED_OF_SOUND 1500  // Speed of sound in water (m/s)

float estimate_delta_tof(const float *corr_output, int length) {
    int max_index = 0;
    float max_value = corr_output[0];

    for (int i = 1; i < length; i++) {
        if (corr_output[i] > max_value) {
            max_value = corr_output[i];
            max_index = i;
        }
    }

    int lag = max_index - (length / 2);
    return (float)lag / FS;
}

float calculate_flow_velocity(float delta_tof) {
    float base_tof = SENSOR_DISTANCE / SPEED_OF_SOUND;
    return (SENSOR_DISTANCE * delta_tof) / (base_tof * base_tof);
}

int main() {
    FILE *file = fopen("cross_correlation_output.txt", "r");

    float cross_corr_output[2 * 512 - 1];
    for (int i = 0; i < (2 * 512 - 1); i++) {
        fscanf(file, "%f", &cross_corr_output[i]);
    }
    fclose(file);

    float delta_tof = estimate_delta_tof(cross_corr_output, 2 * 512 - 1);
    float flow_velocity = calculate_flow_velocity(delta_tof);

    printf("Estimated Delta TOF: %f seconds\n", delta_tof);
    printf("Estimated Flow Velocity: %f m/s\n", flow_velocity);

    return 0;
}
