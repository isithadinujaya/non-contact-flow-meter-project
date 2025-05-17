import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import butter, filtfilt, correlate, correlation_lags, windows

# 1) Simulation parameters
f0 = 1e6                # Carrier freq: 1 MHz
fs = 10e6               # Sampling: 10× carrier for fine TOF resolution 
cycles = 20             # Burst length: 20 cycles for narrow correlation main lobe 
c = 1480                # Speed of sound in water (m/s) 
L = 0.1                 # Transducer separation (m)
flow_rate = 0.2         # Flow speed (m/s)

# 2) Derived parameters
Ts = 1 / fs
burst_dur = cycles / f0
N = int(burst_dur * fs)
t = np.arange(N) * Ts

# 3) Transmit burst: Blackman‐Harris windowed sine
tx = np.sin(2*np.pi*f0*t) * windows.blackmanharris(N)  # high side‐lobe suppression 

# 4) True TOFs (down/up) and fractional delays
tof_down = L / (c + flow_rate)
tof_up   = L / (c - flow_rate)
d_down = tof_down * fs
d_up   = tof_up   * fs

# 5) Symmetric zero‐padding around tx
pad = int(np.ceil(max(d_down, d_up))) + N
zero_pad = np.zeros(pad)
tx_pad = np.concatenate([zero_pad, tx, zero_pad])      # spans full lag range 

# 6) Fractional delay via FFT-based phase shift
def frac_delay(sig, delay):
    M = len(sig)
    S = np.fft.fft(sig, n=2*M)
    f  = np.fft.fftfreq(2*M, Ts)
    H  = np.exp(-2j*np.pi*f*delay*Ts)
    y  = np.fft.ifft(S * H)
    return np.real(y[:M])

# 7) Generate received signals
rx_down = frac_delay(tx_pad, d_down) * 0.5
rx_up   = frac_delay(tx_pad, d_up)   * 0.5

# 8) Band-pass filter design (0.8–1.2 MHz)
b, a = butter(4, [0.8e6/(fs/2), 1.2e6/(fs/2)], btype='bandpass')  # 4th-order Butterworth 
rx_down_f = filtfilt(b, a, rx_down)
rx_up_f   = filtfilt(b, a, rx_up)

# 9) Cross‐correlation & parabolic interpolation
def estimate_tof(rx, tx, fs):
    corr = correlate(rx, tx, mode='full')
    lags = correlation_lags(len(rx), len(tx), mode='full')
    i0 = np.argmax(corr)
    y_m1, y0, y_p1 = corr[i0-1], corr[i0], corr[i0+1]
    delta = (y_m1 - y_p1) / (2*(y_m1 - 2*y0 + y_p1))              # parabolic interpolator 
    return (lags[i0] + delta) / fs, corr, lags

est_td, corr_d, lags_d = estimate_tof(rx_down_f, tx_pad, fs)
est_tu, corr_u, lags_u = estimate_tof(rx_up_f,   tx_pad, fs)

# 10) Flow‐rate calculation
v_est = (L/2) * (1/est_tu - 1/est_td)                           # differential TOF cancels c 

# --- Plotting ---
plt.figure(figsize=(12,8))

# Transmit pulse
plt.subplot(3,1,1)
plt.plot((np.arange(len(tx_pad))-pad)/fs*1e6, tx_pad)
plt.title('Windowed Transmit Burst (Blackman-Harris)')
plt.xlabel('Time (µs)'); plt.ylabel('Amplitude')

# Filtered receptions
plt.subplot(3,1,2)
plt.plot((np.arange(len(rx_down_f))-pad)/fs*1e6, rx_down_f, label='Downstream')
plt.plot((np.arange(len(rx_up_f))-pad)/fs*1e6, rx_up_f,   label='Upstream', alpha=0.7)
plt.title('Filtered Received Signals')
plt.xlabel('Time (µs)'); plt.legend()

# Correlations
plt.subplot(3,1,3)
plt.plot(lags_d/fs*1e6, corr_d, label='Corr Down')
plt.plot(lags_u/fs*1e6, corr_u, label='Corr Up')
plt.axvline(est_td*1e6, color='b', linestyle='--', label=f'Down TOF={est_td*1e6:.2f}µs')
plt.axvline(est_tu*1e6, color='r', linestyle='--', label=f'Up TOF={est_tu*1e6:.2f}µs')
plt.title('Cross-correlations & TOF Estimates')
plt.xlabel('Lag (µs)'); plt.legend(); plt.tight_layout()

plt.show()

print(f"True TOF Down: {tof_down*1e6:.2f} µs, Est: {est_td*1e6:.2f} µs")
print(f"True TOF Up:   {tof_up*1e6:.2f} µs, Est: {est_tu*1e6:.2f} µs")
print(f"Estimated Flow Velocity: {v_est:.3f} m/s (True: {flow_rate:.3f} m/s)")
