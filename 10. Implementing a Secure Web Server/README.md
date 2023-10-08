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

# [Alternative TLS]

## [Forward Proxy]

- Sit between **clients and the internet**
- Acts on behalf of client to foraward their requests to external servers

ex) VPN etc

## [Reverse Proxy]

- Sit between **the internet and backend server**
- Handle incoming client requests and route them to the appropriate backend servers

+) nginx, load balancer etc

+) Q) Warning site that indicates governement prohibition or censorship

- **DPI (Depp Packet Inspection)** : local network or at the level of the ISP (Internet Service Protocol) deep packet inspect (DPI) is used to inspect the content of the data packets transmitted in the request

### [1. Reverse Proxy : Nginx]

- Accept secure connections from clients
- forward these connections as plain HTTP to your program
- This way is also called a **TLS Termination Proxy**

## [CGI (Common Gateway Interface), FastCGI]

- Mechanism used to interface web servers with external applications or scripts
- FastCGI : extension of CGI that addresses some of its performance limitation

### [2. Custom CGI or FastCGI]

- Your program communicates directly with a standard web server
- The web server handles all of the HTTPS and HTTP details


**+) WSGI**

- Without Nginx or a similar web server acting as a reverse proxy to handle SSL/TLS termination, the responsibility for handling TLS/SSL encryption and decryption typically falls directly on the WSGI server itself. 
- This means that the WSGI server needs to be configured to handle HTTPS traffic, including certificate management and encryption.

<br/>

- Popular WSGI servers like Gunicorn and uWSGI can be configured to support HTTPS, but this approach is less common in production deployments because it may **not provide the same level of security, performance, and flexibility as using a dedicated web server like Nginx or Apache for SSL/TLS termination.**
