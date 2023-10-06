# [HTTPS (Hypertext Markup Language Secure)]

- `TLS` is the successor to `Secure Socket Layer (SSL)` an earlier protocol also used by HTTPS

1. `Authentication` : TLS provides peer authentication methods for this reason
2. `Encryption` : TLS uses encryption to obfuscate transmitted data
3. `Integrity` : TLS also ensures that received data has not been tampered with or otherwise forged

- `TLS` : most commonly used for HTTPS security
- secure many other TCP-based protocols

# [Encryption bascis]

- Method of encoding data so that only authorized parties can access it
- Encryption algorithm = **ciphers** > **ciphertext**

- Symmetric Cipher : same key
- Assymetric cipher : 2 different key

## [Symmetric Cipher]

- A scret key is used with the cipher to produce a ciphertext

<br/>

- **American Encryption Standard (AES)**
- Camellia
- **Data Encryiption Standard (DES)**
- Triple DES
- **International Data Encryption Algorithm (IDEA)**
- QUAD
- RC4
- Salsa20, Chacha20
- **Tiny Encryption Algorithm (TEA)**

<br/>

- The most common key exchange algorithm in use today is the **Diffie-Hellman key exchange algorithm**

<br/>

+) **man-in-the-middle**

- In the case of interception, an attacker could stand in the middle of a key exchange, while posing as each corresponding party

## [Asymmetric ciphers]

- public-key encryption, attempts to solve the key exchange and authentication problems of symmetric encryption

<br/>

- One key can encrypt data, while the other key can decrypt it (Public Key, Private Key)
- These key are generated together and related mathmatically

<br/>

- `Rivest-Shamir-Adleman (RSA)` cipher is one of the first public-key ciphers
- `Elliptic-Curve Cryptography (ECC)` cipher promise greater efficiency and are quickly gaining market share


# [How TLS uses ciphers]

- `TLS` can also use digital signatures to authenticate the client
- Asymmetric encryption could be used to protect an entire communication channel

<br/>

- In practice, **modern asymmetric ciphers are inefficient and only able to protect small amounts of data**
- For this reason, **symmetric ciphers are preferred whenever possible**
- TLS uses asymmetric ciphers only to authenticate the server

<br/>

+) **cipher suites** : `key exchange algorithm`, `bulk encryption algorithm`, `message authenication algorithm (MAC)`

- When a TLS connection is first established, the TLS client sends a list of perferred cipher suites to the server
- The TLS server will select one of these cipher suites to be used for the connection
- If the client and server do not support any cipher suite in common, then no TLS connection can be established

# [Certificates]

- Each HTTPS server uses one or more certificates to verify their identity
- Certificate chains can be several layers deep
- As long as you can verify digital signature back to a certificate you trust, you are able to validate the whole chain

<br/>

- This method is the common one used by HTTPS to authenticate servers
- `IdenTrust`, `Comodo`, `DigiCert`, `GoDaddy`, `GlobalSign` : precceding certificate authorities are responsible for over 90% of the HTTPS certificates found on the web

<br/>

- Certificates are usually matched to domain names, also possible for them to identify other information

<br/>

- Is's common today for one server to host many different domains and each domain requires its own certificate


# [SNI (Server name identification)]

- TLS connection should be established before the HTTP data is sent

- Therefore the **server must decide which certificate to transmit before the HTTP Host header is received**

ex) 1 host, multiple server = same ip and multiple certification
=> client don't know specific certification they need

- A technique that when used by TLS requires the client to indicate to the server which domain it is attmpting to conenct
- The server can then find a matching certificate to use for the TLS connection

<br/>

- SNI is relatively new and older browsers and servers do not support it
- Before SNI was popular servers had 2 choices, they could either host only one domain per IP address, or they could send certificates for all hosted domains for each connection

<br/>

- Commonly Client initiates the connection to a server so already know the server's hostname
- However some scenario where the client might not have prior knowledge of the server's information

<br/>

- When you open a web browser and **access a website via HTTPS**, your **browser sends an SSL/TLS handshake request that often includes the SNI extension**. This is a standard practice for modern web browsers.

<br/>

- It should be noted that SNI involves sneding the unencrypted domain name over the network

## [ESNI (Encrypted Server Name Identification)]

- not widely deployed yet

<br/>

> With a basic understanding of the TLS protocol, we;re ready to look at the most popular library, **OpenSSL**


### +) SSH (Secure Shell)

- `Application layer & Transport layer` and a suite of tools used for secure remote access
- Desgin to provide strong encryption, authentication and integrity

<br/>

- Do not perform the same 3 way handshake as the TCP/IP 
- But `SSH handshake`, which is part of the SSH protocol's initial connection setup
- **This handshake takes place over the SSH transport layer not the application layer**
- Provide secure communication by encrypting and transmitting data over a secure channel using a protocol like TCP

### +) MAC Algorithm (Message Authentication Code)

- MAC algorithm (e.g., HMAC (Hash based Message Authentication Code) with SHA-256)
	- Combines SHA-256 (hash algorithm) with a secret key to generate MAC tags for message authentication
	- MAC algorithm is not hash algorithm

- Use shared secret key and the chosen MAC algorithm to compute the MAC tag for the message
- Use shared secret key and the same MAC algorithm to compute a MAC tag for the received message


## OpenSSL

- Widely used open source library that provides SSL and TLS services to applications