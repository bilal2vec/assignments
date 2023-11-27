import control as ct
import matplotlib.pyplot as plt
import math
ct.use_matlab_defaults()

s = ct.tf('s')

G = 20 / (s + 1) / (s + 2)

# controllers to improve phase margin
# (1) lower gain (naive solution)
C = 10
# (2) lag compensator
alpha = 10
mu = 10
omega_c = 4.2
T = 10 / omega_c
# C = mu * (1 + T * s) / (1 + alpha * T * s)

plt.figure()
ct.bode_plot(G, margins=True, omega_limits=[0.01, 100])
ct.bode_plot(C * G, margins=True, omega_limits=[0.01, 100])

plt.figure()
t, y = ct.step_response(G / (1 + G))
_, y_c = ct.step_response(C * G / (1 + C * G), t)
plt.plot(t, y, label='w/o compensator')
plt.plot(t, y_c, label='w/ compensator')
plt.grid()
plt.legend()

plt.show()