#ifndef CORRELATION_H
#define CORRELATION_H

int cross_correlate(const float *x, const float *y, int length);
float compute_velocity_from_delay(int delay_samples);

#endif
