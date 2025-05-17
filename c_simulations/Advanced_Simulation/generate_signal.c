// generate_signal.c

#include <stdio.h>
#include <math.h>

#define FS 1000000         // Sampling rate (1 MHz)
#define SIGNAL_LENGTH 512  // Number of samples in one signal
#define SIGNAL_FREQ 40000  // Frequency of the signal (40 kHz)

void generate_transmitter_signal(float *signal) {
    for (int i = 0; i < SIGNAL_LENGTH; i++) {
        signal[i] = sinf(2 * M_PI * SIGNAL_FREQ * i / FS);
    }
}

int main() {
    float tx_signal[SIGNAL_LENGTH];
    generate_transmitter_signal(tx_signal);

    // Output the generated signal (Optional: Save to a file for visualization)
    FILE *file = fopen("tx_signal.txt", "w");
    for (int i = 0; i < SIGNAL_LENGTH; i++) {
        fprintf(file, "%f\n", tx_signal[i]);
    }
    fclose(file);

    printf("Transmitter signal generated and saved to tx_signal.txt\n");
    return 0;
}
