import math
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

mu = 4
C_1 = mu

pole_1 = 10**0
pole_2 = 10**2

L_star = 5 / ((1 + (s / pole_1)) * (1 + (s / pole_2))**2)
C_2 = L_star * (1 / G)
C = C_1 * C_2

# C(s) Bode plot
plt.figure()
ct.bode_plot(C * G, margins=True)
plt.show()

# C(s) step response
plt.figure()
t, y = ct.step_response((C * G) / (1 + (C * G)))
plt.plot(t, y)
plt.show()

# Steady State Output at end of step response
print(f"Steady State Output: {y[-1]}")
# Prints out
# Steady State Output: 0.9523809526136096
