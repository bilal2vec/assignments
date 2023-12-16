import numpy as np

class SystemModel:

    def __init__(self, z0=[]):
        self.dt = 0.001
        self.d = 10.0
        if z0 == []:
            self.z = np.array([0.0, 0.0, 0.0, 0.0])
        else:
            self.z = z0

    def step(self, u):
        self.z = self.z + 0.1665834 * np.array([
            (self.z[2]) * self.dt + 2.0 * ((self.z[2] + (-self.d * self.z[2]**3 + u[0]) * self.dt / 2.0) * self.dt + (self.z[2] + (-self.d * (self.z[2] + (-self.d * self.z[2]**3 + u[0]) * self.dt / 2.0)**3 + u[0]) * self.dt / 2.0) * self.dt) + (self.z[2] + (-self.d * (self.z[2] + (-self.d * (self.z[2] + (-self.d * self.z[2]**3 + u[0]) * self.dt / 2.0)**3 + u[0]) * self.dt / 2.0)**3 + u[0]) * self.dt) * self.dt,
            (self.z[3]) * self.dt + 2.0 * ((self.z[3] + (-self.d * self.z[3]**3 + u[1]) * self.dt / 2.0) * self.dt + (self.z[3] + (-self.d * (self.z[3] + (-self.d * self.z[3]**3 + u[1]) * self.dt / 2.0)**3 + u[1]) * self.dt / 2.0) * self.dt) + (self.z[3] + (-self.d * (self.z[3] + (-self.d * (self.z[3] + (-self.d * self.z[3]**3 + u[1]) * self.dt / 2.0)**3 + u[1]) * self.dt / 2.0)**3 + u[1]) * self.dt) * self.dt,
            (-self.d * self.z[2]**3 + u[0]) * self.dt + 2.0 * ((-self.d * (self.z[2] + (-self.d * self.z[2]**3 + u[0]) * self.dt / 2.0)**3 + u[0]) * self.dt + (-self.d * (self.z[2] + (-self.d * (self.z[2] + (-self.d * self.z[2]**3 + u[0]) * self.dt / 2.0)**3 + u[0]) * self.dt / 2.0)**3 + u[0]) * self.dt) + (-self.d * (self.z[2] + (-self.d * (self.z[2] + (-self.d * (self.z[2] + (-self.d * self.z[2]**3 + u[0]) * self.dt / 2.0)**3 + u[0]) * self.dt / 2.0)**3 + u[0]) * self.dt)**3 + u[0]) * self.dt,
            (-self.d * self.z[3]**3 + u[1]) * self.dt + 2.0 * ((-self.d * (self.z[3] + (-self.d * self.z[3]**3 + u[1]) * self.dt / 2.0)**3 + u[1]) * self.dt + (-self.d * (self.z[3] + (-self.d * (self.z[3] + (-self.d * self.z[3]**3 + u[1]) * self.dt / 2.0)**3 + u[1]) * self.dt / 2.0)**3 + u[1]) * self.dt) + (-self.d * (self.z[3] + (-self.d * (self.z[3] + (-self.d * (self.z[3] + (-self.d * self.z[3]**3 + u[1]) * self.dt / 2.0)**3 + u[1]) * self.dt / 2.0)**3 + u[1]) * self.dt)**3 + u[1]) * self.dt
        ])
        z_noisy = self.z + 0.01 * np.random.normal(0.0, 0.1, size=self.z.shape)
        return z_noisy[:2]

    def sim(self, input_signal):
        N = input_signal.shape[0]
        y = np.zeros((N, 2))
        for n in range(N):
            y_t = self.step(input_signal[n, :])
            y[n, :] = y_t
        return y

sm = SystemModel()

T = 15
dt = 0.001
steps = int(T / dt)

# Item 1

u = np.ones((steps, 2))

u_1 = u[:, 0]
u_2 = u[:, 1]

y = sm.sim(u)

y_1 = y[:, 0]
y_2 = y[:, 1]

# Item 2

import matplotlib.pyplot as plt

plt.plot(np.arange(0, T, dt), u_1)
plt.title('Input Signal 1 (u1)')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude / Magnitude')

plt.plot(np.arange(0, T, dt), u_2)
plt.title('Input Signal 2 (u_2)')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude / Magnitude')

plt.plot(np.arange(0, T, dt), y_1)
plt.title('Output Signal 1 (y_1)')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude / Magnitude')

plt.plot(np.arange(0, T, dt), y_2)
plt.title('Output Signal 2 (y_2)')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude / Magnitude')

# Item 3

y_1_fft = np.fft.fft(y_1)
y_2_fft = np.fft.fft(y_2)

freqs = np.fft.fftfreq(steps, dt)

plt.plot(freqs[:len(freqs) // 2], np.abs(y_1_fft[:len(freqs) // 2]))
plt.title('y_1 FFT')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Amplitude / Magnitude')

plt.plot(freqs[:len(freqs) // 2], np.abs(y_2_fft[:len(freqs) // 2]))
plt.title('y_2 FFT')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Amplitude / Magnitude')

# Item 4

plt.plot(np.log(freqs[:len(freqs) // 2]), np.abs(y_1_fft[:len(freqs) // 2]))
plt.title('y_2 FFT (log scale)')
plt.xlabel('log Frequency (Hz)')
plt.ylabel('Amplitude / Magnitude')

# Compute tau and convert chosen freq from hz to rad/s
chosen_freq = np.e

# Item 5

omega_c = 2 * np.pi * chosen_freq
tau = 1 / omega_c

# Compute s, our inputs to the transfer functions. Note that our freqs from
# np.fft.fftfreq are in units of hz and so we convert then to j * w in rad/s too
s = 1j * 2 * np.pi * freqs

G_s = 1 / ((1 + tau * s) ** 3)

# Item 6

y_1_fft_filtered = G_s * y_1_fft
y_2_fft_filtered = G_s * y_2_fft

y_1_filtered = np.fft.ifft(y_1_fft_filtered)
y_2_filtered = np.fft.ifft(y_2_fft_filtered)

plt.plot(np.arange(0, T, dt), y_1_filtered, label="y_1 (filtered)")
plt.plot(np.arange(0, T, dt), y_1, label="y_1")
plt.title('y_1 original vs filtered')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude / Magnitude')
plt.legend()

