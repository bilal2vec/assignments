from functools import reduce

SBOX_SIZE = 4


def to_bin(n, digits=4):
    return bin(n)[2:].rjust(digits, '0')


def from_bin(s):
    return int(s, 2)


class SBox():
    sub_mat = {
        0: 14,
        1: 4,
        2: 13,
        3: 1,
        4: 2,
        5: 15,
        6: 11,
        7: 8,
        8: 3,
        9: 10,
        10: 6,
        11: 12,
        12: 5,
        13: 9,
        14: 0,
        15: 7,
    }
    inv_mat = {}
    for k, v in sub_mat.items():
        inv_mat[v] = k

    @classmethod
    def apply(cls, n):
        return to_bin(cls.sub_mat[from_bin(n)])

    @classmethod
    def invert(cls, n):
        return to_bin(cls.inv_mat[from_bin(n)])


def xor(a, b):
    ''' xor two bit strings. '''
    assert len(a) == len(b)
    assert all(c in '01' for c in a)
    assert all(c in '01' for c in b)

    def xor_char(c1, c2):
        return str(int(c1 != c2))

    return ''.join(xor_char(c1, c2) for c1, c2 in zip(a, b))


def relation_2ab(U, P):
    return (U[5], U[7], U[13], U[15], P[4], P[6], P[7])


def relation_2d(U, P):
    return (P[0], P[3], P[8], P[11], U[1], U[5], U[9], U[13])


def format_key(key, bit_guesses):
    out = ''
    guess_idx = 0
    for c in key:
        if c == '?':
            out += bit_guesses[guess_idx]
            guess_idx += 1
        else:
            out += c
    return out


def find_bias(plaintexts, ciphertexts, key, relation):
    relation_holds_count = 0
    total = 0
    for P, C in zip(plaintexts, ciphertexts):
        U = ''

        # Construct U from reversing the s-boxes
        # There are 4 s-boxes, each with 4 bits of input
        for start_bit in range(0, 4 * SBOX_SIZE, SBOX_SIZE):
            key_bits = key[start_bit:start_bit + SBOX_SIZE]
            if any(c not in '01' for c in key_bits):
                # We don't known all the key bits for this s-box
                U += '?' * SBOX_SIZE
                continue
            cipher_bits = C[start_bit:start_bit + SBOX_SIZE]
            sbox_output = xor(cipher_bits, key_bits)
            U += SBox.invert(sbox_output)
        total += 1

        relation_holds = reduce(xor, relation(U, P)) == '0'
        if relation_holds:
            relation_holds_count += 1
    return abs(0.5 - (relation_holds_count / total))


def find_key(plaintexts, ciphertexts, key, relation):
    # Get which bits we are looking for
    unknown_bits = [i for i, c in enumerate(key) if c == '?']
    # So, we basically have to guess each possible key and find the bias for
    # Each one.
    max_bias = 0
    best_guess = None
    for n in range(2**len(unknown_bits)):
        bin_guess = to_bin(n, digits=len(unknown_bits))
        key_guess = format_key(key, bin_guess)
        # Now we have a full key, compute the bias for it.
        bias = find_bias(plaintexts, ciphertexts, key_guess, relation)
        print('Key {} has bias {}'.format(key_guess, bias))
        if bias > max_bias:
            max_bias = bias
            best_guess = key_guess
    return best_guess, max_bias


def main():
    with open('a2q1plaintexts.txt') as pf, open('a2q1ciphertexts.txt') as cf:
        p_texts = list(map(str.strip, pf))
        c_texts = list(map(str.strip, cf))

    bias = find_bias(p_texts, c_texts, 'xxxx0111xxxx0110', relation_2ab)
    print(f'{bias=}')
    best_key, bias = find_key(p_texts, c_texts, 'xxxx????xxxx????',
                              relation_2ab)
    next_key = best_key.replace('x', '?')
    total_key, end_bias = find_key(p_texts, c_texts, next_key, relation_2d)

    print('Question 2a: The bias is {}'.format(str(bias)))
    print('Question 2b: The best key guess is {} with bias {}'.format(
        best_key, bias))
    print('Question 2d: The final key bits are {} with bias {}'.format(
        total_key, end_bias))


if __name__ == '__main__':
    main()
