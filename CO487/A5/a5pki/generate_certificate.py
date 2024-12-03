from datetime import datetime
import json
import sys
from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric.utils import decode_dss_signature
from cryptography.hazmat.primitives import serialization


# Helper function for converting strings into byte arrays needed by cryptographic functions
def string_to_bytes(s):
    return s.encode('utf-8')


# This function will ensure that we represent the JSON dictionary as exactly the
# same string every time, otherwise we'd get different hashes while signing
def canonicalize_json(j):
    return json.dumps(j, sort_keys=True)


# Parse the command-line arguments
if len(sys.argv) != 6:
    print(
        "Usage: generate_certificate.py ca_identity subject_identity validity_start validity_end usage"
    )
    print("       The files ca_identity.sk and subject_identity.pk must exist")
    print("       Output will be written to subject_identity.cert")
    sys.exit(-1)
certificate_authority_identity = sys.argv[1]
subject_identity = sys.argv[2]
validity_start = datetime.fromisoformat(sys.argv[3])  # Use ISO format for dates
validity_end = datetime.fromisoformat(sys.argv[4])
usage = sys.argv[5]

# Load the subject's public key
with open(subject_identity + ".pk", "r") as subject_pk_file:
    subject_pk_text = subject_pk_file.read()

# Hold the body of the certificate as a dictionary.
# When we output it to a file, we'll save it in JSON format
body = {
    "subject identity": subject_identity,
    "public key": subject_pk_text,
    "validity start": validity_start.isoformat(),
    "validity end": validity_end.isoformat(),
    "issuer identity": certificate_authority_identity,
    "key usage": usage
}
# Format the certificate body for signing as a byte array in a canonical order
cert_body_to_be_signed = string_to_bytes(canonicalize_json(body))

# Sign the body of the certificate

# Load the issuer's signing key
with open(certificate_authority_identity + ".sk", "r") as sk_file:
    sk_lines = sk_file.read()
signing_key = serialization.load_pem_private_key(string_to_bytes(sk_lines),
                                                 password=None)

# Sign the body of the certificate as a json string with ECDSA and SHA256
# Formatting will have to be the same when verification happens
signature = signing_key.sign(cert_body_to_be_signed, ec.ECDSA(hashes.SHA256()))
(r, s) = decode_dss_signature(signature)

# Construct the certificate as the body & the signature
certificate = {"body": body, "signature": {'r': r, 's': s}}

with open(subject_identity + '.cert', "w") as cert_file:
    cert_file.write(json.dumps(certificate, indent=4))
