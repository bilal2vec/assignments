import hashlib


def decrypt():
    hash = "303b8f67ffbe7f7d5e74e9c2177cd1cabbc9dc53154199f540e1901591c7d5fa"
    salt = "19147384"

    for i in range(1000000):
        password = f"{i:06d}"

        computed_hash = hashlib.sha256((salt + password).encode()).hexdigest()

        # Check if hash matches
        if computed_hash == hash:
            print(password)
            return

    return False


decrypt()
