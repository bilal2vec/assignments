import control as ct
import matplotlib.pyplot as plt
ct.use_matlab_defaults()

s = ct.tf('s')
G = 10 / (1 + 10 * s) / (1 + 5 * s) / (1 + s)

# designed controller
C = (1 + 10 * s) * (1 + 5 * s) * (1 + s) / (1 + s / 0.03) / (1 + s / 3) ** 2

L = C * G

# ct.pzmap(ct.minreal(L / (1 + L)))
# plt.show()

ct.bode_plot(G, margins=True)
ct.bode_plot(L, margins=True)

_, ax_step = plt.subplots(1)
t, y_G = ct.step_response(G / (1 + G))
t, y_L = ct.step_response(L / (1 + L), t)
ax_step.plot(t, y_G, label='w/o controller')
ax_step.plot(t, y_L, label='w/ controller')
ax_step.grid()
plt.legend()
plt.show()