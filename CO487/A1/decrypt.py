with open('./a1q1ciphertexts/ctxt1.txt', 'r') as file:
    ciphertext = file.read()

shift = 5
decrypted = ""
for char in ciphertext:
    shift_amount = shift % 26
    new_char = chr((ord(char) - shift_amount - 65) % 26 + 65) 
    decrypted += new_char

print(decrypted)
print()
