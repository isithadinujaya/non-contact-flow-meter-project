#include <math.h>
#include "correlation.h"

// Cross-correlation function
int cross_correlate(const float *x, const float *y, int length) {
    float max_corr = -1.0f;
    int best_lag = 0;

    for (int lag = -length / 2; lag < length / 2; lag++) {
        float sum = 0.0f;
        for (int i = 0; i < length; i++) {
            int j = i + lag;
            if (j >= 0 && j < length) {
                sum += x[i] * y[j];
            }
        }
        if (sum > max_corr) {
            max_corr = sum;
            best_lag = lag;
        }
    }
    return best_lag;
}

float compute_velocity_from_delay(int delay_samples) {
    const float sample_interval = 1.0 / 1e6; // 1 MHz sampling
    const float delay_time = delay_samples * sample_interval;
    const float distance_between_sensors = 0.05f; // 5 cm
    return distance_between_sensors / delay_time;
}
