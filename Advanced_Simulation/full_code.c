#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846
#define FS 1000000          // 1 MHz sampling rate
#define SIGNAL_LENGTH 512   // Samples
#define SIGNAL_FREQ 40000   // 40 kHz ultrasonic signal
#define SENSOR_DISTANCE 0.1 // 10 cm
#define SPEED_OF_SOUND 1500 // Speed of sound in water (m/s)

// Function to generate a sinewave transmitter signal
void generate_transmitter_signal(float *signal) {
    for (int i = 0; i < SIGNAL_LENGTH; i++) {
        signal[i] = sinf(2 * PI * SIGNAL_FREQ * i / FS);
    }
}

// Function to generate a received signal based on flow velocity
void generate_received_signal(float *tx_signal, float *rx_signal, float flow_velocity) {
    float base_tof = SENSOR_DISTANCE / SPEED_OF_SOUND;
    float adjusted_tof = SENSOR_DISTANCE / (SPEED_OF_SOUND + flow_velocity);
    float delta_tof = base_tof - adjusted_tof;
    int delay_samples = (int)(delta_tof * FS);

    for (int i = 0; i < SIGNAL_LENGTH; i++) {
        if (i - delay_samples >= 0 && (i - delay_samples) < SIGNAL_LENGTH) {
            rx_signal[i] = tx_signal[i - delay_samples];
        } else {
            rx_signal[i] = 0.0f;
        }
    }
}

// Function to perform cross-correlation
void cross_correlation(const float *x, const float *y, float *corr_output, int length) {
    for (int lag = -length + 1; lag < length; lag++) {
        float sum = 0.0f;
        for (int i = 0; i < length; i++) {
            int j = i - lag;
            if (j >= 0 && j < length) {
                sum += x[i] * y[j];
            }
        }
        corr_output[lag + length - 1] = sum;
    }
}

// Function to estimate delta TOF from cross-correlation
float estimate_delta_tof(const float *corr_output, int length) {
    int max_index = 0;
    float max_value = corr_output[0];

    for (int i = 1; i < length; i++) {
        if (corr_output[i] > max_value) {
            max_value = corr_output[i];
            max_index = i;
        }
    }

    int lag = max_index - (SIGNAL_LENGTH - 1);
    float delta_tof = (float)lag / FS;
    return delta_tof;
}

// Function to calculate flow velocity
float calculate_flow_velocity(float delta_tof) {
    float base_tof = SENSOR_DISTANCE / SPEED_OF_SOUND;
    float velocity = (SENSOR_DISTANCE * delta_tof) / (base_tof * base_tof);
    return velocity;
}

int main() {
    float tx_signal[SIGNAL_LENGTH];
    float rx_signal[SIGNAL_LENGTH];
    float cross_corr_output[2 * SIGNAL_LENGTH - 1];

    float flow_velocity_true = 2.0f; // 2 m/s

    // Generate signals
    generate_transmitter_signal(tx_signal);
    generate_received_signal(tx_signal, rx_signal, flow_velocity_true);

    // Perform cross-correlation
    cross_correlation(rx_signal, tx_signal, cross_corr_output, SIGNAL_LENGTH);

    // Estimate delta TOF
    float delta_tof_est = estimate_delta_tof(cross_corr_output, 2 * SIGNAL_LENGTH - 1);

    // Calculate flow velocity
    float flow_velocity_est = calculate_flow_velocity(delta_tof_est);

    // Print results
    printf("True Flow Velocity: %.6f m/s\n", flow_velocity_true);
    printf("Estimated Flow Velocity: %.6f m/s\n", flow_velocity_est);

    return 0;
}
