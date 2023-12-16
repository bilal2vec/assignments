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