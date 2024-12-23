import jwt
from datetime import datetime
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.backends import default_backend

# Private key
private_key_pem = """-----BEGIN PRIVATE KEY-----
MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQC6maOpC+TRzdNs
j6ko8TUH+ycstVYOaT57zaKcAgAhcvNoIHB9psUZPxhsgho8feTZrx62HZMkU+M3
3Kf9NJ654Po3nfRtEDTMl2qCIoe/mnYx1ZwVvMVWlDxTmSgsvCCSq6jrR1jO1G1g
9D3mnw2+r4AZMrb52btlELR5/WsL0Y/vy1nHmjuh9M74se0k1D2TODJpr1OuIctp
mSH6x/0Ay9Ocya2ghCuLgvDAX8xipwGlwNoG7RLJZa6tNjJ/1glmARPjcvH3aKRQ
l9LGCe8WZh772t0VcZRWgVJrpdRnCBpu+xY62xo7wD8M42PRyLIijpCMTs3tACIP
LStb1sVrAgMBAAECggEARcR/GAv95GHgHHWESm4RJWhGp+UvBVldRt3GYC0xTlBT
6MlENf6+L+fS4nMsixSJFcjvoyjgpJMG4BKM7tOXSyludJAG3SZTj3FHndt6RO3k
JlLXNptrbykieyFHD57RUPWExV9aOY7cTIVVhZ5Pfc3Rh9pZ5oX9llaOHBRfyz1O
aPrkbQdjlNi7fNMP/zp/+Fry/lsLHFLWP0O4/iJ75/K+oGBhoysD9x9qOmUbsfv+
icSuhabi/QqESVkhNsT9FQBJ24qKLpxYbtN44rs+Rh2u3+FzFSoY33yC/TtXinyC
tSrdTtOWnCIzPZaBhebEsIgU5gLxOqA9UpITfRYjVQKBgQDfbUshWjp/0dORRCXs
M8/87/pJXvMnNhkPmAcWOxuifTKRo2yKH+W3I3bAmi1K6uwhwLbAK6rLc75HQrhR
oi5VS39L9jXolPLuXmbxEms0CKdI9Y5GJ0JOu7DsEwAAkhDM33/aNU/1pQlL8ct2
XcDt9QeMCu9dvqdTI3FJ/tStFQKBgQDVzecGURLayuXJPxyD4yHGZ1DebehN4vWf
3YXR1FAAqUFLWB4CdN0raeM68twsBvUz4m+MXNhi/WYSiWrKSN7MxoAETfUFEXci
6AV0Vyc4yRMXAzVkLOIklv3SDABk1Zj5zmvog00PNKQOga9qOoI3E3/4tDZBxciX
el6JYMtIfwKBgQCoo1vDztC8zWk9Cgz9nZXtOkzVuF8bR4/e30bqFq8jFBBpMMqp
7q16b8AgW763/10/lK/bmJoPeksyij+JDjqekp+AfFPwOwypsoNMjjRwT9/EB+Pd
O6CmwLAnX6fuxlD78e4Ui4zQwkKkGdsSem02rm2F3HMTdOXisKu/yCJKiQKBgEww
VZVAqtFg4EW4wbaV5+FFy+GXlOK8b/8J97fwSpoRtnzc58TEVFQ2JY1KIf9mc1aL
6GhCZbHQzuDoKy/ui8a5LcFa7mdh8+xydg8hR47KyIy4zfvmqqeTTtvc/v7vxtWu
I3H16C30+klZGWNneiOaRU4hGfySOIsSRkH6RjaNAoGBAMPM7tXbsETa9pTgfgyY
lY8hLp+YADxqiPMAMm9S5e8TbYMeGtp5mh3fuHfIKRdJAh7+9GykALh/hATNJ7iR
8mc1BY+6JmPhcJqauZa/TNpFpKxXrZ2fna9zgQOklQ5Xz8ImV75Oy7oL0M8Wks7I
OlCOfxqvgIcMsmEO/P5iFxM3
-----END PRIVATE KEY-----"""

# Load the private key
private_key = serialization.load_pem_private_key(
    private_key_pem.encode(),
    password=None,
    backend=default_backend()
)

# Header
header = {
    "alg": "RS256",
    "typ": "JWT"
}

# Payload
payload = {
    "hw_info": {
        "motherboard_serial": "/7Z38YQ3/CNWS20025Q004P/",
        "hdd_serial": "WD-WCC6Y0LPFK0T",
        "windows_product_id": "00331-10000-00001-AA636",
        "bios_uuid": "4C4C4544-005A-3310-8038-B7C04F595133",
        "tpm_endorsement_key": "Not Available",
        "cpu_id": "BFEBFBFF00090672"
    },
    "exp": 1735766400  # Expiration timestamp
}

# Generate the JWT
jwt_token = jwt.encode(
    payload,
    private_key,
    algorithm="RS256",
    headers=header
)

print("Generated JWT:")
print(jwt_token)
