// cross_correlation.c

#include <stdio.h>
#include <math.h>

#define SIGNAL_LENGTH 512   // Number of samples
#define MAX_LAG (2 * SIGNAL_LENGTH - 1)

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

int main() {
    float tx_signal[SIGNAL_LENGTH], rx_signal[SIGNAL_LENGTH];
    float cross_corr_output[MAX_LAG];
    
    // Read received signal from file
    FILE *file = fopen("rx_signal.txt", "r");
    for (int i = 0; i < SIGNAL_LENGTH; i++) {
        fscanf(file, "%f", &rx_signal[i]);
    }
    fclose(file);

    // Read transmitter signal from file
    file = fopen("tx_signal.txt", "r");
    for (int i = 0; i < SIGNAL_LENGTH; i++) {
        fscanf(file, "%f", &tx_signal[i]);
    }
    fclose(file);

    // Perform cross-correlation
    cross_correlation(rx_signal, tx_signal, cross_corr_output, SIGNAL_LENGTH);

    // Output the cross-correlation result (Optional: Save to a file)
    FILE *output = fopen("cross_correlation_output.txt", "w");
    for (int i = 0; i < MAX_LAG; i++) {
        fprintf(output, "%f\n", cross_corr_output[i]);
    }
    fclose(output);

    printf("Cross-correlation performed and saved to cross_correlation_output.txt\n");
    return 0;
}
