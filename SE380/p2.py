import numpy as np
import matplotlib.pyplot as plt
import control as ct
from control.matlab import *


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


##############
# TASK 1
##############

# sim parameters
T = 15
dt = 0.001
steps = int(T / dt)

# calculate y_11 and y_21
sm = SystemModel()

u_11 = np.ones(steps)
u_21 = np.zeros(steps)
u = np.vstack((u_11, u_21)).T
y = sm.sim(u)

y_11 = y[:, 0]
y_21 = y[:, 1]

# calculate y_12 and y_22
sm = SystemModel()

u_12 = np.zeros(steps)
u_22 = np.ones(steps)
u = np.vstack((u_12, u_22)).T
y = sm.sim(u)

y_12 = y[:, 0]
y_22 = y[:, 1]


##############
# TASK 2
##############

# choose filter frequency and create transfer function for it
w_c = 100
tau = 1 / w_c

s = ct.tf('s')
t = np.linspace(0, (steps - 1) * dt, steps)


G_s = 1 / ((1 + tau * s) ** 3)

# simulate affect of filter on y_11, y_12, y21, y22
y_11_filtered = lsim(G_s, y_11, t)[0]
y_12_filtered = lsim(G_s, y_12, t)[0]
y_21_filtered = lsim(G_s, y_21, t)[0]
y_22_filtered = lsim(G_s, y_22, t)[0]


##############
# TASK 3
##############

# calulate first derivative of y_11, y_12, y21, y22
y_11_filtered_1 = np.diff(y_11_filtered).T/dt
y_12_filtered_1 = np.diff(y_12_filtered).T/dt
y_21_filtered_1 = np.diff(y_21_filtered).T/dt
y_22_filtered_1 = np.diff(y_22_filtered).T/dt

# calculate second derivative of y_11, y_12, y21, y22
y_11_filtered_2 = np.diff(y_11_filtered_1).T/dt
y_12_filtered_2 = np.diff(y_12_filtered_1).T/dt
y_21_filtered_2 = np.diff(y_21_filtered_1).T/dt
y_22_filtered_2 = np.diff(y_22_filtered_1).T/dt

# calulate first derivate of u_11, u_12, u_21, u_22
u_11_1 = np.diff(u_11).T/dt
u_12_1 = np.diff(u_12).T/dt
u_21_1 = np.diff(u_21).T/dt
u_22_1 = np.diff(u_22).T/dt


##############
# TASK 4
##############

fig = plt.figure(figsize = (24, 12))
ax1 = fig.add_subplot(221)
ax2 = fig.add_subplot(222)
ax3 = fig.add_subplot(223)
ax4 = fig.add_subplot(224)
ax1.plot(t, y_11)
ax1.title.set_text('y_11')
ax2.plot(t, y_12)
ax2.title.set_text('y_12')
ax3.plot(t, y_21)
ax3.title.set_text('y_21')
ax4.plot(t, y_22)
ax4.title.set_text('y_22')

fig = plt.figure(figsize = (24, 12))
ax1 = fig.add_subplot(221)
ax2 = fig.add_subplot(222)
ax3 = fig.add_subplot(223)
ax4 = fig.add_subplot(224)
ax1.plot(t, y_11_filtered)
ax1.title.set_text('y_11 filtered')
ax2.plot(t, y_12_filtered)
ax2.title.set_text('y_12 filtered')
ax3.plot(t, y_21_filtered)
ax3.title.set_text('y_21 filtered')
ax4.plot(t, y_22_filtered)
ax4.title.set_text('y_22 filtered')

fig = plt.figure(figsize = (24, 12))
ax1 = fig.add_subplot(221)
ax2 = fig.add_subplot(222)
ax3 = fig.add_subplot(223)
ax4 = fig.add_subplot(224)
ax1.plot(np.arange(0, T-dt, dt), y_11_filtered_1)
ax1.title.set_text('y_11 filtered first derivative')
ax2.plot(np.arange(0, T-dt, dt), y_12_filtered_1)
ax2.title.set_text('y_12 filtered first derivative')
ax3.plot(np.arange(0, T-dt, dt), y_21_filtered_1)
ax3.title.set_text('y_21 filtered first derivative')
ax4.plot(np.arange(0, T-dt, dt), y_22_filtered_1)
ax4.title.set_text('y_22 filtered first derivative')

fig = plt.figure(figsize = (24, 12))
ax1 = fig.add_subplot(221)
ax2 = fig.add_subplot(222)
ax3 = fig.add_subplot(223)
ax4 = fig.add_subplot(224)
ax1.plot(np.arange(0, T-2*dt, dt), y_11_filtered_2)
ax1.title.set_text('y_11 filtered second derivative')
ax2.plot(np.arange(0, T-2*dt, dt), y_12_filtered_2)
ax2.title.set_text('y_12 filtered second derivative')
ax3.plot(np.arange(0, T-2*dt, dt), y_21_filtered_2)
ax3.title.set_text('y_21 filtered second derivative')
ax4.plot(np.arange(0, T-2*dt, dt), y_22_filtered_2)
ax4.title.set_text('y_22 filtered second derivative')


##############
# TASK 5
##############

# function to compute d_t
def d_t(step_index, u, u_1, y, y_1):
    return np.vstack((u[step_index], u_1[step_index], -y[step_index], -y_1[step_index])).T


# function to compute theta
def theta(time_steps, u, u_1, y, y_1, y_2):
    y_bar = np.zeros(time_steps)
    D_bar = np.zeros((time_steps, 4))
    for i in range(0, time_steps-2):
        y_bar[i] = y_2[i]
        D_bar[i] = d_t(i, u, u_1, y, y_1)
    return np.linalg.lstsq(D_bar, y_bar)[0]


#calulate thetas
theta_11 = theta(steps, u_11, u_11_1, y_11_filtered, y_11_filtered_1, y_11_filtered_2)
theta_12 = theta(steps, u_12, u_12_1, y_12_filtered, y_12_filtered_1, y_12_filtered_2)
theta_21 = theta(steps, u_21, u_21_1, y_21_filtered, y_21_filtered_1, y_21_filtered_2)
theta_22 = theta(steps, u_22, u_22_1, y_22_filtered, y_22_filtered_1, y_22_filtered_2)


# function to get transfer function from theta
def theta_to_g(theta):
    return (theta[1]*s + theta[0])/(s**2 + theta[3]*s + theta[2])


# compute transfer functions
g_11 = theta_to_g(theta_11)
g_12 = theta_to_g(theta_12)
g_21 = theta_to_g(theta_21)
g_22 = theta_to_g(theta_22)

# print out transfer functions
print(g_11)
print(g_12)
print(g_21)
print(g_22)


##############
# TASK 6
##############

# compute simulated output of transfer function on unit step
y_11_id = lsim(g_11, np.ones(steps), t)[0]
y_12_id = lsim(g_12, np.ones(steps), t)[0]
y_21_id = lsim(g_21, np.ones(steps), t)[0]
y_22_id = lsim(g_22, np.ones(steps), t)[0]

fig = plt.figure(figsize = (24, 12))
ax1 = fig.add_subplot(221)
ax2 = fig.add_subplot(222)
ax3 = fig.add_subplot(223)
ax4 = fig.add_subplot(224)
ax1.plot(t, y_11)
ax1.plot(t, y_11_id)
ax1.title.set_text('y_11 identified vs real')
ax2.plot(t, y_12)
ax2.plot(t, y_12_id)
ax2.title.set_text('y_12 identified vs real')
ax3.plot(t, y_21)
ax3.plot(t, y_21_id)
ax3.title.set_text('y_21 identified vs real')
ax4.plot(t, y_22)
ax4.plot(t, y_22_id)
ax4.title.set_text('y_22 identified vs real')


##############
# TASK 7
##############

# calculate y_11 and y_21 response to sin input
sm = SystemModel()

u_11_sin = np.sin(t)
u_21_sin = np.zeros(steps)
u = np.vstack((u_11_sin, u_21_sin)).T
y = sm.sim(u)

y_11_sin = y[:, 0]
y_21_sin = y[:, 1]


# calculate y_12 and y_22 response to sin input
sm = SystemModel()

u_12_sin = np.zeros(steps)
u_22_sin = np.sin(t)
u = np.vstack((u_12_sin, u_22_sin)).T
y = sm.sim(u)

y_12_sin = y[:, 0]
y_22_sin = y[:, 1]


# compute simulated output of transfer function on sin
y_11_sin_id = lsim(g_11, np.sin(t), t)[0]
y_12_sin_id = lsim(g_12, np.sin(t), t)[0]
y_21_sin_id = lsim(g_21, np.sin(t), t)[0]
y_22_sin_id = lsim(g_22, np.sin(t), t)[0]

fig = plt.figure(figsize = (24, 12))
ax1 = fig.add_subplot(221)
ax2 = fig.add_subplot(222)
ax3 = fig.add_subplot(223)
ax4 = fig.add_subplot(224)
ax1.plot(t, y_11_sin)
ax1.plot(t, y_11_sin_id)
ax1.title.set_text('y_11 sin input identified vs real')
ax2.plot(t, y_12_sin)
ax2.plot(t, y_12_sin_id)
ax2.title.set_text('y_12 sin input identified vs real')
ax3.plot(t, y_21_sin)
ax3.plot(t, y_21_sin_id)
ax3.title.set_text('y_21 sin input identified vs real')
ax4.plot(t, y_22_sin)
ax4.plot(t, y_22_sin_id)
ax4.title.set_text('y_22 sin input identified vs real')


plt.show()
