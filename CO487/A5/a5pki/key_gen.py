import os
import sys
from cryptography.hazmat.primitives.asymmetric import ec 
from cryptography.x509 import ObjectIdentifier
from cryptography.hazmat.primitives import serialization

# Helper function for converting byte arrays needed by cryptographic functions back into strings
def bytes_to_string(b):
    return b.decode('utf-8')

# Parse the command-line arguments
if len(sys.argv) != 2:
    print("Usage: key_gen.py username")
    print("Will generate public key / private key pair for this user and save to username.pk / username.sk")
    sys.exit(1)
identity = sys.argv[1]

# Use the following elliptic curve as our ECDSA parameters
curve = ec.SECP256R1()

# Generate key pair
signing_key = ec.generate_private_key(curve)
public_key = signing_key.public_key()

# Save the signing key
with open(identity + '.sk', 'w') as sk_file:
    sk_file.write(bytes_to_string(signing_key.private_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PrivateFormat.PKCS8,
        encryption_algorithm=serialization.NoEncryption()
    )))

# Save the public key
with open(identity + '.pk', 'w') as pk_file:
    pk_file.write(bytes_to_string(public_key.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )))
