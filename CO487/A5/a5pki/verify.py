from datetime import datetime
import json
import sys
from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric.utils import encode_dss_signature
from cryptography.hazmat.primitives import serialization


# Helper function for converting strings into byte arrays needed by cryptographic functions
def string_to_bytes(s):
    return s.encode('utf-8')


# This function will ensure that we represent the JSON dictionary as exactly the
# same string every time, otherwise we'd get different hashes while signing
def canonicalize_json(j):
    return json.dumps(j, sort_keys=True)


def verify(ca_identity, signed_message_filename):

    print("Trying to verify " + signed_message_filename)

    # Load the signed message data
    with open(signed_message_filename, 'r') as fh:
        signed_message = json.load(fh)

    # Read out the identity of the signer and load their certificate
    signer_identity = signed_message['signer identity']
    with open(signer_identity + '.cert', 'r') as fh:
        signer_cert = json.load(fh)
    # Format the certificate body for signing as a byte array in a canonical order
    cert_body_to_be_signed = string_to_bytes(
        canonicalize_json(signer_cert["body"]))

    # Read out the identity of the issuer and load their public key
    issuer_identity = signer_cert['body']['issuer identity']
    signer_pk = serialization.load_pem_public_key(
        string_to_bytes(signer_cert['body']['public key']))
    with open(ca_identity + '.pk', 'r') as fh:
        ca_public_key = serialization.load_pem_public_key(
            string_to_bytes(fh.read()))

    try:
        if issuer_identity != ca_identity:
            raise Exception('issuer_identity != ca_identity')

        cert_signature = encode_dss_signature(signer_cert['signature']['r'],
                                              signer_cert['signature']['s'])

        try:
            ca_public_key.verify(cert_signature, cert_body_to_be_signed,
                                 ec.ECDSA(hashes.SHA256()))
        except Exception as e:
            raise Exception('ca_public_key.verify=False')

        current_time = datetime.now()
        validity_start = datetime.fromisoformat(
            signer_cert['body']['validity start'])
        validity_end = datetime.fromisoformat(
            signer_cert['body']['validity end'])

        if current_time < validity_start or current_time > validity_end:
            raise Exception(
                'current_time < validity_start or current_time > validity_end')

        message_signature = encode_dss_signature(
            signed_message['signature']['r'], signed_message['signature']['s'])

        try:
            signer_pk.verify(message_signature,
                             string_to_bytes(signed_message['message']),
                             ec.ECDSA(hashes.SHA256()))
        except Exception as e:
            raise Exception('signer_pk.verify=False')

        print('verified')
        return True
    except Exception as e:
        print('not verified: ' + str(e))
        return False


verify("dstebila", "message1.signed.txt")
verify("dstebila", "message2.signed.txt")
verify("dstebila", "message3.signed.txt")
verify("dstebila", "message4.signed.txt")
verify("dstebila", "message5.signed.txt")
verify("dstebila", "message6.signed.txt")
