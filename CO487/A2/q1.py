import numpy as np
from tqdm import tqdm

s_box = np.array([14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7])

s_box_inv = np.zeros(16, dtype=np.int32)
for i in range(16):
    s_box_inv[s_box[i]] = i


def read_file(file_path):
    with open(file_path, 'r') as file:
        lines = [line.strip() for line in file]

    blocks = []
    for line in lines:
        blocks.append([int(line[i:i + 4], 2) for i in range(0, 16, 4)])

    return np.array(blocks)


def calculate_bias(plaintexts, ciphertexts, key_guess):
    count = 0
    for plaintext, ciphertext in zip(plaintexts, ciphertexts):
        U = calculate_U_values(ciphertext, key_guess)
        P = calculate_P_values(plaintext)

        #        linear_approximation = (U[5] ^ U[7] ^ U[13] ^ U[15] ^ P[4] ^ P[6] ^
        #                                P[7])
        linear_approximation = (U[0] ^ U[5] ^ U[9] ^ U[13] ^ P[0] ^ P[3] ^
                                P[8] ^ P[11])

        if linear_approximation == 0:
            count += 1

    bias = abs(count / len(plaintexts) - 0.5)
    return bias


def calculate_U_values(ciphertext, key_guess):
    U_blocks = [None] * 4
    for i in range(4):
        U_blocks[i] = s_box_inv[np.bitwise_xor(ciphertext[i], key_guess[i])]

    U = np.zeros(16, dtype=np.int32)
    for i in range(16):
        U[i] = U_blocks[i // 4] >> (3 - i % 4) & 1

    return U


def calculate_P_values(plaintext):
    P = np.zeros(16, dtype=np.int32)
    for i in range(16):
        P[i] = int(plaintext[i // 4] >> (3 - i % 4) & 1)
    return P


plaintexts = read_file('./a2q1plaintexts.txt')
ciphertexts = read_file('./a2q1ciphertexts.txt')


def calculate_key(key_guess):
    binary_to_decimal = np.array([2**3, 2**2, 2**1, 2**0])
    blocks = key_guess.reshape(4, 4)
    blocks_decimal = np.dot(blocks, binary_to_decimal)
    return blocks_decimal


guess = np.zeros(16, dtype=np.int32)
guess[4] = 0
guess[5] = 1
guess[6] = 1
guess[7] = 1
guess[12] = 0
guess[13] = 1
guess[14] = 1
guess[15] = 0

guess_decimal = calculate_key(guess)

bias = calculate_bias(plaintexts, ciphertexts, guess_decimal)
print(f"Bias: {bias}")


def find_best_key(plaintexts, ciphertexts):
    max_bias = 0
    best_subkey = None

    for subkey in tqdm(range(256)):
        guess = np.zeros(16, dtype=np.int32)
        guess[4] = (subkey >> 7) & 1
        guess[5] = (subkey >> 6) & 1
        guess[6] = (subkey >> 5) & 1
        guess[7] = (subkey >> 4) & 1
        guess[12] = (subkey >> 3) & 1
        guess[13] = (subkey >> 2) & 1
        guess[14] = (subkey >> 1) & 1
        guess[15] = subkey & 1

        guess_decimal = calculate_key(guess)

        bias = calculate_bias(plaintexts, ciphertexts, guess_decimal)

        if bias > max_bias:
            max_bias = bias
            best_subkey = guess.copy()

    return best_subkey, max_bias


best_subkey, max_bias = find_best_key(plaintexts, ciphertexts)
print(f"Best Subkey: {best_subkey}")
print(f"Max Bias: {max_bias}")
