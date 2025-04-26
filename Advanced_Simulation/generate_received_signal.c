// generate_received_signal.c

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FS 1000000         // Sampling rate (1 MHz)
#define SIGNAL_LENGTH 512  // Number of samples in one signal
#define SENSOR_DISTANCE 0.1  // 10 cm distance between sensors
#define SPEED_OF_SOUND 1500  // Speed of sound in water (m/s)

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

int main() {
    float tx_signal[SIGNAL_LENGTH], rx_signal[SIGNAL_LENGTH];
    float flow_velocity = 2.0f; // 2 m/s (adjust this to test different flow rates)

    // Read transmitter signal from file (if saved in the previous step)
    FILE *file = fopen("tx_signal.txt", "r");
    for (int i = 0; i < SIGNAL_LENGTH; i++) {
        fscanf(file, "%f", &tx_signal[i]);
    }
    fclose(file);

    generate_received_signal(tx_signal, rx_signal, flow_velocity);

    // Output the received signal (Optional: Save to a file for visualization)
    FILE *output = fopen("rx_signal.txt", "w");
    for (int i = 0; i < SIGNAL_LENGTH; i++) {
        fprintf(output, "%f\n", rx_signal[i]);
    }
    fclose(output);

    printf("Received signal generated and saved to rx_signal.txt\n");
    return 0;
}
