import matplotlib.pyplot as plt
import numpy as np

sm = SystemModel()
sm.z = np.vstack(
    (np.random.uniform(low=-2.0, high=2.0, size=(2, 1)), np.zeros(
        (2, 1))))  # initialize state of the system
y = sm.step(np.zeros((2, 1)))  # get initial robot position

delta_t = sm.dt

ctrl = Controller(delta_t)

T = 30
n_steps = int(T / delta_t)

xs = np.linspace(0, T, n_steps)
ys = np.zeros((n_steps, 2))
us = np.zeros((n_steps, 2))

for i in range(n_steps):
    u = ctrl.calculate_acceleration(y)
    y = sm.step(u)

    ys[i, :] = y.reshape(2,)
    us[i, :] = u.reshape(2,)

plt.figure(figsize=(12, 8))

plt.subplot(2, 1, 1)
plt.plot(xs, ys[:, 0], label='$y_1$')
plt.plot(xs, ys[:, 1], label='$y_2$')
plt.title('y_1, y_2 vs time')
plt.xlabel('Time (s)')
plt.ylabel('Output')
plt.legend()

plt.subplot(2, 1, 2)
plt.plot(xs, us[:, 0], label='$u_1$')
plt.plot(xs, us[:, 1], label='$u_2$')
plt.title('Input Signals Over Time')
plt.xlabel('Time (s)')
plt.ylabel('Input')
plt.legend()

plt.tight_layout()
plt.show()
