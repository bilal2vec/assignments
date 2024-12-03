import json
import sys
from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric.utils import decode_dss_signature
from cryptography.hazmat.primitives import serialization

# Helper function for converting strings into byte arrays needed by cryptographic functions
def string_to_bytes(s):
    return s.encode('utf-8')

# Parse the command-line arguments
if len(sys.argv) != 4:
    print("Usage: sign.py signer_identity input_filename output_filename")
    sys.exit(1)
signer_identity = sys.argv[1]
input_filename = sys.argv[2]
output_filename = sys.argv[3]

# Load the signer's secret key
with open(signer_identity + ".sk", 'r') as sk_file:
    sk_contents = sk_file.read()
signing_key = serialization.load_pem_private_key(string_to_bytes(sk_contents), password=None)

# Load the message to be signed
with open(input_filename, 'r') as input_file:
    message = input_file.read()

# Sign the message
# note that we had to convert the message to be signed to a byte array; make sure you 
# do the same thing during verification
signature = signing_key.sign(string_to_bytes(message), ec.ECDSA(hashes.SHA256()))
# get the r and s values of the ECDSA signature
(r, s) = decode_dss_signature(signature)

# Save the signature to a file in a JSON data structure
output = {
    'message': message,
    'signer identity': signer_identity,
    'signature': {'r': r, 's': s}
}
with open(output_filename, 'w') as output_file:
    output_file.write(json.dumps(output, indent=4))
