import numpy as np

n = 4
q = 3329

c1 = np.array([220, 1211, 2479, 1812])
c2 = 1799
s = np.array([-1, 1, 1, 0])

tmp = (c2 - np.dot(c1, s)) % q
m_hat = abs(tmp / (q // 2))

print(f"m = {m_hat}")
