import numpy as np                             # array handling, window functions :contentReference[oaicite:0]{index=0}
from scipy import signal                       # cross-correlation and lag utilities :contentReference[oaicite:1]{index=1}

# 1. Load or simulate your echo waveforms
# Here we generate two identical pulses, one delayed by 50 samples
fs = 10e6                                      # Sampling rate: 10 MHz :contentReference[oaicite:2]{index=2}
t = np.arange(0, 1e-3, 1/fs)                   # 1 ms of samples :contentReference[oaicite:3]{index=3}
pulse = np.sin(2*np.pi*1e6*t) * np.hanning(len(t))  # 1 MHz tone windowed by Hann :contentReference[oaicite:4]{index=4}
delay_samples = 50                             # True delay in samples
echo_up = np.roll(pulse, delay_samples)        # Upstream echo :contentReference[oaicite:5]{index=5}
echo_down = pulse                              # Downstream echo

# 2. Preprocess: windowing and DC removal
window = np.hanning(len(echo_up))              # Hanning window :contentReference[oaicite:6]{index=6}
echo_up *= window                              # Apply window :contentReference[oaicite:7]{index=7}
echo_down *= window                            # Same for downstream
echo_up -= np.mean(echo_up[:100])              # Remove DC using first 100 samples :contentReference[oaicite:8]{index=8}
echo_down -= np.mean(echo_down[:100])          # Likewise

# 3. Cross-correlation via FFT
corr = signal.correlate(echo_up, echo_down, mode='full', method='fft')  # FFT-based correlation :contentReference[oaicite:9]{index=9}
lags = signal.correlation_lags(len(echo_up), len(echo_down), mode='full')  # Compute lag indices :contentReference[oaicite:10]{index=10}

# 4. Find the integer-sample peak
peak_idx = np.argmax(corr)                     # Index of maximum correlation :contentReference[oaicite:11]{index=11}
int_delay = lags[peak_idx]                     # Integer-sample delay estimate

# 5. Sub-sample quadratic interpolation
y_m1 = corr[peak_idx - 1]                      # Value at peak-1
y_0  = corr[peak_idx]                          # Value at peak
y_p1 = corr[peak_idx + 1]                      # Value at peak+1
delta = (y_p1 - y_m1) / (2*(y_p1 + y_m1 - 2*y_0))# Quadratic interpolation :contentReference[oaicite:12]{index=12}
precise_delay = int_delay + delta              

# 6. Convert delay to time and velocity
dt = precise_delay / fs                         # Time-of-flight difference Δt :contentReference[oaicite:13]{index=13}
D = 0.1                                        # Acoustic path length (m) – set to your geometry
theta = np.deg2rad(45)                         # Beam angle (45°) – set to your setup
t_down = None; t_up = None                     # If using two separate TOFs, assign accordingly
# For symmetric pulses, the two-way method simplifies to:
v = (D * delta) / (2 * np.cos(theta) * (len(echo_up)/fs)**2)  # Simplified for illustration :contentReference[oaicite:14]{index=14}

# 7. Output results
print(f"Integer delay: {int_delay} samples")  
print(f"Sub-sample correction: {delta:+.3f} samples")  
print(f"Precise delay: {precise_delay:.3f} samples → Δt = {dt*1e9:.2f} ns")  
print(f"Estimated velocity: {v:.3f} m/s")
