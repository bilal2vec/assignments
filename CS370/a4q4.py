# -*- coding: utf-8 -*-
"""Untitled13.ipynb

Automatically generated by Colaboratory.

Original file is located at
    https://colab.research.google.com/drive/1NpiHw7khVo4UOxuDIScSdWQpAQXQWCxU
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy.sparse import csr_matrix

def PageRank(G, alpha):
    it = 0
    delta = 1
    tol = 1e-8

    R = G.shape[0]
    p_k_1 = p_0 = np.ones(R) / R

    d = (np.sum(G, axis=0) == 0).astype(int)

    P = (G + (1 / R) * d.transpose())
    P_prime = P / np.sum(P, axis=0)

    M = alpha * P_prime + (1 - alpha) * (1 / R)

    while delta > tol:
        p_k = M @ p_k_1
        delta = np.linalg.norm(p_k - p_k_1, 1)
        p_k_1 = p_k
        it += 1

    return p_k, it

def PageRankSparse(G, alpha):
    it = 0
    delta = 1
    tol = 1e-8

    R = G.shape[0]
    p_k_1 = p_0 = np.ones(R) / R

    d = (np.array(G.sum(axis=0)).flatten() == 0).astype(int)

    P = G + d
    P_prime = P / P.sum(axis=0)

    while delta > tol:
        p_k = alpha * np.array(P_prime.dot(p_k_1)).flatten() + (1 - alpha) * (1 / R)
        delta = np.linalg.norm(p_k - p_k_1, 1)
        p_k_1 = p_k
        it += 1

    return p_k, it

G = np.array([[0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0],
              [1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
              [0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0],
              [0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0],
              [0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0],
              [0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0],
              [0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
              [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
              [0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0],
              [0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1],
              [0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0]])
alpha = 0.9

(p, it) = PageRank(G, alpha)
print(p, it)

plt.spy(G)
plt.title("Graph Sparsity Pattern")

plt.bar(range(len(p)), p)
plt.title("PageRank Scores")
plt.xlabel("Page")
plt.ylabel("PageRank Score")

(p, it) = PageRankSparse(csr_matrix(G), alpha)
print(p, it)

plt.spy(csr_matrix(G))
plt.title("Graph Sparsity Pattern")

plt.bar(range(len(p)), p)
plt.title("PageRank Scores")
plt.xlabel("Page")
plt.ylabel("PageRank Score")

import scipy.io

data = scipy.io.loadmat('bbc.mat')

Gcsr = data['G']
Gcsr = Gcsr.transpose() #data uses the reverse adjacency matrix convention.
U = data['U']

(p, it) = PageRankSparse(Gcsr, alpha)
print(p, it)

plt.spy(Gcsr)
plt.title("Graph Sparsity Pattern")

plt.bar(range(len(p)), p)
plt.title("PageRank Scores")
plt.xlabel("Page")
plt.ylabel("PageRank Score")

[val[0][0] for val in U[np.argsort(p)[-20:]]]
