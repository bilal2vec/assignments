import string


def oplus(left, right):
    length = len(left)
    if len(right) != length:
        raise RuntimeError('adder length inconsistent')
    s = ""
    for i in range(0, length):
        if left[i] == right[i]:
            s += "0"
        else:
            s += "1"
    return s


def out_sbox(output):  # input as 4-bit string
    if output == '1110':
        return '0000'
    elif output == '0100':
        return '0001'
    elif output == '1101':
        return '0010'
    elif output == '0001':
        return '0011'
    elif output == '0010':
        return '0100'
    elif output == '1111':
        return '0101'
    elif output == '1011':
        return '0110'
    elif output == '1000':
        return '0111'
    elif output == '0011':
        return '1000'
    elif output == '1010':
        return '1001'
    elif output == '0110':
        return '1010'
    elif output == '1100':
        return '1011'
    elif output == '0101':
        return '1100'
    elif output == '1001':
        return '1101'
    elif output == '0000':
        return '1110'
    elif output == '0111':
        return '1111'


def calc_bias_more(K1, K2, K3, K4, pcdic):
    count = 0
    for p in pcdic:
        c = pcdic[p]
        C1 = c[0:4]
        C2 = c[4:8]
        C3 = c[8:12]
        C4 = c[12:16]

        V1 = oplus(K1, C1)
        V2 = oplus(K2, C2)
        V3 = oplus(K3, C3)
        V4 = oplus(K4, C4)

        U1 = out_sbox(V1)
        U2 = out_sbox(V2)
        U3 = out_sbox(V3)
        U4 = out_sbox(V4)

        U42 = int(U1[1])
        U46 = int(U2[1])
        U410 = int(U3[1])
        U414 = int(U4[1])

        P1 = int(p[0])
        P4 = int(p[3])
        P9 = int(p[8])
        P12 = int(p[11])

        if (U42 + U46 + U410 + U414 + P1 + P4 + P9 + P12) % 2 == 0:
            count += 1

    return ((K1, K2, K3, K4), (count - 10000) / 20000.00)


plaintext = open('a2q1plaintexts.txt')
ciphertext = open('a2q1ciphertexts.txt')

try:
    pdic = plaintext.readlines()
    cdic = ciphertext.readlines()
finally:
    plaintext.close()
    ciphertext.close()

length = len(pdic)
pcdic = {}

for i in range(0, length):
    key = pdic[i][0:16]
    val = cdic[i][0:16]
    pcdic[key] = val

subkeys = []
for k1 in range(0, 16):
    K1 = '{0:04b}'.format(k1)
    for k3 in range(0, 16):
        K3 = '{0:04b}'.format(k3)
        subkeys.append((K1, '0001', K3, '1011'))

biases = []

for subkey in subkeys:
    biases.append(
        calc_bias_more(subkey[0], subkey[1], subkey[2], subkey[3], pcdic))

for b in biases:
    print(b)

max = (('', '', '', ''), 0)
for b in biases:
    bias = abs(b[1])
    if bias >= abs(max[1]):
        max = b

print('Max: ' + str(max))
"""
length = len(biases)
constant = 1/32.0
for i in range(0, length):
    biases[i] = abs(biases[i])
    biases[i] = abs(biases[i] - constant)



class diffpair:
    # diff
    # left
    # right

    def __init__(self, d, l, r):
        self.diff = d
        self.left = l
        self.right = r

    def print(self):
        print('diff: ' + self.diff + ', left: ' + self.left + ', right: ' + self.right)
"""
"""
pdiffdic = []

for i in range(0, length):
    for j in range(i + 1, length):
        temp = oplus(pdic[i], pdic[j])
        if pick_at(temp, [5, 7, 8]):
            pdiffdic.append(diffpair(temp, pdic[i], pdic[j]))
        if len(pdiffdic) >= 100:
            break
    if len(pdiffdic) >= 100:
        break

"""
