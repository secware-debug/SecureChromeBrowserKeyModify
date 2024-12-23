from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives import serialization

# Generate private key
private_key = rsa.generate_private_key(
    public_exponent=65537,
    key_size=2048,  # Key size in bits (2048 is secure and commonly used)
)

# Serialize private key to PEM format
private_key_pem = private_key.private_bytes(
    encoding=serialization.Encoding.PEM,
    format=serialization.PrivateFormat.PKCS8,
    encryption_algorithm=serialization.NoEncryption()  # No password for simplicity
)

# Save the private key to a file (optional)
with open("private_key.pem", "wb") as f:
    f.write(private_key_pem)

# Generate public key from private key
public_key = private_key.public_key()

# Serialize public key to PEM format
public_key_pem = public_key.public_bytes(
    encoding=serialization.Encoding.PEM,
    format=serialization.PublicFormat.SubjectPublicKeyInfo
)

# Save the public key to a file (optional)
with open("public_key.pem", "wb") as f:
    f.write(public_key_pem)

# Print keys
print("Private Key:")
print(private_key_pem.decode())
print("\nPublic Key:")
print(public_key_pem.decode())
