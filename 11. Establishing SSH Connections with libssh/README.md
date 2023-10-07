# [The SSH protocol]

## [Telnet]

- A client remotely connects to a server using plaintext over TCP port 23
- The server provides more-or-less direct access to the OS command-line through this `TCP (Transmission Control Protocol)`
- But Does `not encrypt any data sent over the network`

## [SSH protocol]
- Allow clients to verify servers' identities using public-key authentication

<br/>

# [libssh]

- widely used open source C library implementing the SSH protocol
- I allows us to remotely execute commands and transfer files using the SSH protocol

## [VPS (Virtual Private Server)]

- Virtualized server that runs the Linux OS

### [VPS vs VM]

**VPS**

- lightweight and resource-efficient from of virtualization
- containerization or OS-level virtualization to create isolated instances
- Resources are shared among the virtual servers on the same physical server

**VM**

- Hardware-level virtualization
- Has its own complete OS and runs on a hypervisor
- Each VM has its own dedicated set of resources

<br/>

# [SSH Authentication]

- SSH provides authentication methods for both the server and the client
- The server wants to only provide access to authorized users

<br/>

- SSH doesn't typically rely on certificate authorities
- When using SSH, most clients simply keep a list of the public keys that they trust
- `libssh` implements features to remember trusted servers' public keys

<br/>

- Some SSH deployments also use other methods to validate SSH hosts' public keys

+) **Secure Shell FingerPrinter (SSFP)**

- type of a DNS record used to validate SSH public keys

<br/>

# [Client Authentication]

- **No Authentication** : This allows any user to connect
- **Password Authentication** : This requires the user to provide a username and password
- **Public Key** : This uses public key encryption methods to authenticate
- **Keyboard-interactive** : Having the user answer several prompts
- **Generic Security Service Application Program Interface (GSS-API)** : A variety of other services

