# DNS (Domain Name Service)

## Query

- ISP (Internet Service Provider) / Many DNS Server
- They cache the host in local cache

## Record

- `A`, `AAA` : IPv4, IPv6
- `MX` : Mail Server
- `TXT` : Arbitart information for a hostname
- `CNAME` : Alias, SubDomain
- `*, ANY, ALL` : All know record types for the current query

## Security

- `UDP` over `53 port`
- **TCP fallback, DNS Transport Over TCP** : If the query in too large to fit into on UDP packet, the query can be performed over TCP instead of UDP

## DNS Server IP

- Usually provide by ISP

## Endianess

- BidEndian

<br/>

# Mail Server

## Protocol

- Delivering : SMTP (Mail Transmission) 25 port
- Receiving
  - POP : Download email to a local device
  - IMAP : Access, View and Manage emails directly
  - HTTP, HTTPS : ex) gmail

### Secure

- SMTP upgrade `STARTTLS`
  - Upgrade to a secure encryption connection
  - Start SMTP command through TLS layer

- Port : 465 (SMTP + SSL), 587 (SMTP + TLS)

## Finding Server

- DNS lookup for an MX record

## Spam blocking pitfalls

- `Sender Policy Framework (SPF)` : if a sending server isn't on the SPF list, then receiving SMTP server reject their mail
- `Domain Keys Identified Mail (DKIM)` : Authenticate email using digital signature
  - Treat non-DKIM email as spam
  - Generate DKIM key using email server
  - Issued by domain name (Copy and Paste Domain TXT record that public DKIM key)
- `Domain-based Message Authentication, Reporting and Conformance (DMARC)`  : Publish & Check SPF, DKIM

<br/>

# HTTPS

## TLS
- Successor to SSL (Secure Socket Layer)
- TCP Based Protocol

## Asymmetric > Symmetric cipher
- Asymmetric ciphers are inefficient
- So Secure symmetric cipher (key exchange algorithm, bulk encryption algorithm, message authentication algorithm) using asymmetric cipher
- Send list of performed cipher suites to the server

## SNI (Server Name Identification)
- TLS connection should be established before the HTTP data is sent
- Server must decide which certificate to transmit before the HTTP Host header is received
  - 1 host & multiple service : Same ip have many certification

**[Before SNI]**

1. Only one domain per IP address
2. Send certificate for all hosted domain for each connection

<br/>

# Certification

## TLS Protocol

- Client certificate that they are connected to a trusted server

## Chain of trust model

1. Domain Validation
2. Extended Validation (ex) Personal Information)

## Proxy

### Forward Proxy

- Connect between Client and Internet (ex) VPN)

### Revers Proxy

- Connect between Internet and Server (ex) Nginx (TLS Termination), Load Balancer)

## CGI (Common Gateway Interface), FastCGI

- Custom CGI : Directly with a standard webserver handle all the HTTP & HTTPS

+) WSGI : Can handle HTTPS but not common and have performance problem

<br/>

# SSH Protocol

## Telnet
- Client remotely connects to a server
- Using plaintext over TCP port 23

## SSH (Secure Shell) Protocol
- Client verify Server's identities
- Using public eky authentication
- Caching known hosts
- Using application layer and transport layer
  - After connecting Transport layer do the SSH handshake 
  - Connect using the application layer

<br/>

# Monitor Network

## ICMP (Internet Control Message Protocol)

- Network layer protocol (Just use IP)
- Echo request/response : small & easy to process

## traceroute
- Window : ICMP
- Linux Unix : UDP with not using port 

## netstat




