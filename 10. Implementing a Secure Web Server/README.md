# [Certificates]

- Important part of the TLS protocol
- Although certificates can be used on both the client and server side, `HTTPS generally only uses server certificates`
- These certificates identify to the client that they are connected to a trusted server

### [Chain-of-trust model]

- Each HTTPS client has a few certificate authorities that they explicitly trust and certificate authorities offer services where they digitally sign certificate
- Clients trust the intermediate certificates because they are signed by trusted certificate authorities

### [1. Domain Validation]

- Where a signed certificate is issued after simply verifying that the certificate recipient can be reached at the given domain
- Most common type of validatoin

### [2. Extended Validation (EV)]

- Only issued after the authority veritifies the recipient's identity
- Usually done using public records and a phone call

# [Self-signed certificates with OpenSSL]

- `certificate pinning` : the client can be deployed with a copy of the certificate and trust only that certificate

- certificate pinning can be more secure than using a certificate authority

- Public Web browsers won't trust server

```
openssl req -x509 -newkey rsa:2048 -nodes -sha256 -keyout key.pem -out cert.pem -days 365
```

- `cert.pem` : certificate that gets sent to the connected client
- `key.pem` : provided our server with the enryption key that proves that it owns the certificate


<br/>



