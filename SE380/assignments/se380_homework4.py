import control as ct
import matplotlib.pyplot as plt

# Problem 1
# closed-loop system specifications
#   $|e_\infty| \le 0.05$ in response to a unit step reference
#   $\phi_m \ge 60^\circ$
#   $k_m \ge 10 dB$

omega_n = 10
zeta = 0.1

s = ct.tf('s')
G = 5 / (1 + s) / (1 + 2 * zeta / omega_n * s + s ** 2 / omega_n ** 2)

plt.figure()
ct.bode_plot(G, margins=True)
plt.show()

plt.figure()
t, y = ct.step_response(G / (1 + G))
plt.plot(t, y)
plt.show()

# design your controller here
C = 1.0

L = C * G
plt.figure()
ct.bode_plot(L, margins=True)
plt.show()

plt.figure()
t, y = ct.step_response(L / (1 + L))
plt.plot(t, y)
plt.show()

# Problem 2
# closed-loop system specifications
#   overshoot O.S.\% = 0\%
#   settling time at 1% $T_s^{1\%} \le 0.5s$
#   $|e_\infty| = 0$ in response to a unit step reference

# design your controller here
C = 1.0

L = C * G
plt.figure()
ct.bode_plot(L, margins=True)
plt.show()

plt.figure()
t, y = ct.step_response(L / (1 + L))
plt.plot(t, y)
plt.show()