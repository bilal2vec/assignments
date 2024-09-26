#Your folder contains 4 ciphertexts, each of which is an encryption of some English plaintext written in a similar style of text. The plaintext may start in the middle of a sentence and may end in the middle of a word. Each plaintext is diﬀerent and each is encrypted with a diﬀerent cipher. The four ciphers used, in random order in your set, are:
#
#• shift cipher
#• substitution cipher
#• Vigen`ere cipher, for an unknown block length between 6 and 13
#• transposition cipher, for an unknown block length between 6 and 13
#
#Your task is to determine which cipher was used for each ciphertext, and what the corresponding plaintext is, using the following steps. You may write software to do so in any language of your choice, or use online tools to assist, but please indicate which tool(s) you used. If you write your own software to help you solve, please include that in your submission.

import matplotlib.pyplot as plt
import numpy as np
from collections import Counter

# Read ciphertexts separately
ciphertexts = []
for i in range(4):
    with open(f'./a1q1ciphertexts/ctxt{i}.txt', 'r') as file:
        ciphertexts.append(file.read())

# Count characters for each ciphertext
char_counts = [Counter(ctxt.upper()) for ctxt in ciphertexts]  # Convert to uppercase

# Print character counts for each ciphertext as LaTeX tables
print("\\begin{table}[h]")
print("\\centering")
print("\\begin{tabular}{|c|" + "c|"*26 + "}")
print("\\hline")
print("Ciphertext & " + " & ".join(chr(65+i) for i in range(26)) + " \\\\")
print("\\hline")

for i, counts in enumerate(char_counts):
    row = [str(i)]
    for char in [chr(65+i) for i in range(26)]:
        row.append(str(counts.get(char, 0)))
    print(" & ".join(row) + " \\\\")
    print("\\hline")

print("\\end{tabular}")
print("\\caption{Character frequencies in ciphertexts}")
print("\\label{tab:char_frequencies}")
print("\\end{table}")

# Plot histograms
fig, axs = plt.subplots(2, 3, figsize=(15, 10))
axs = axs.ravel()

for i, counts in enumerate(char_counts):
    chars, freqs = zip(*sorted(counts.items()))
    axs[i].bar(chars, freqs)
    axs[i].set_title(f'Ciphertext {i}')
    axs[i].set_xlabel('Characters')
    axs[i].set_ylabel('Frequency')

# Remove the unused subplot
fig.delaxes(axs[5])

plt.tight_layout()
plt.show()

