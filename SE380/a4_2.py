import math
import numpy as np
import control as ct
import matplotlib.pyplot as plt

omega_n = 10
zeta = 0.1

s = ct.tf('s')
G = 5 / (1 + s) / (1 + 2 * zeta / omega_n * s + s**2 / omega_n**2)

# G(s) Bode plot
plt.figure()
ct.bode_plot(G, margins=True)
plt.show()

# G(s) step response
plt.figure()
t, y = ct.step_response(G / (1 + G))
plt.plot(t, y)
plt.show()

omega_c = 10**1

# We don't rely on C_1 to control the steady-state error
C_1 = 1

pole_1 = 10**0
pole_2 = 10**2
L_star = 5 / ((1 + (s / pole_1)) * (1 + (s / pole_2))**2)
C_2 = L_star * (1 / G)

alpha = 10
mu = 2 * alpha  # the two here is a tunable parameter to control how much to increase steady-state gain
T = 10 / omega_c

c = mu / alpha
C_3 = c + (c / T) * (1 / s)

C = C_1 * C_2 * C_3

# C(s) Bode plot
plt.figure()
ct.bode_plot(C * G, margins=True)
plt.show()

# C(s) step response
plt.figure()
t, y = ct.step_response((C * G) / (1 + (C * G)))
plt.plot(t, y)
plt.show()

# time idx at t = 0.5
idx = np.argmin(np.abs(t - 0.5))

print(f'Overshoot: {1 - max(y)}')
print(f"Max error after settling point (t = 0.5): {max(np.abs(y[idx:] - 1))}")
print(f"Steady-state error: {np.abs(y[-1] - 1)}")

# Prints out:
# Overshoot: -2.8851916411554157e-08
# Max error after settling point (t = 0.5): 0.0019246920997199046
# Steady-state error: 2.1373960601422937e-08
