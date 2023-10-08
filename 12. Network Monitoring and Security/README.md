# [Testing Reachability]

## [1. Ping] 

### [ICMP (Internet Control Message Protocol)]

- Network layer protocol
- Checking the reachability of hosts and whether a server is alive
- **echo request**, **echo reply**
- Echo messages are usually small and easy to process
- This round trip time often serves as a best-case estimate of network latency

## [2. Checking a route : traceroute]

### Packet Type

**[1. ICMP Echo Request (Ping) Packets]**

- Measuring <u>RTT (Round Trip Time)</u> and getting IP address

ex) Window

**[2. UDP Packets]**

- <u>Map the network path</u>
- When a router along the path receives a UDP packet with an expired TTL, it responsds with an ICMP Time Exceeded message, indicating its IP address
- Second probe packet is typically a UDP packet sent to an unused UDP port on the destination port

ex) Mac OS, Linux

### Raw Sockets

- The socket programming APIs we've been working with do not provide access at the IP level that the ICMP is built on
- a C program can construct the exact IP packet to send
- a C programmer could construct an ICMP packet from acratch and send it over the network

<br/>

- Raw sockets aren't universally supported
- Difficult subject to approach in a cross-platform way

## [3. Checking local connections: netstat]

- `-n` : Reverse-DNS lokkups on each address and has the effect of speeding it up significantly

- The queue information, displayed on Unix-based systems represents how many bytes the kernel has queued up waiting for the program to read or the number of byes sent but not acknowloedged by the foreign host

<br/>

## [4. Snooping on connections: tcpdump, wireshark]

- **tcpdump** : commonly used program for packet capture on Unix-based systems
- **Wireshark** : ver ypopular network protocol analyzer that includes a very nice GUI

<br/>

## [5. Capturing all network traffic]

- tshark is only able to see traffic that arrives on your machine
- This usually means you can only use it to monitor traffic from applications on your computer

<br/>

### [1. Using a router that supports mirroring]

- Makes it mirror all traffic to a given Ethernet port
- Mirror all network traffic to a particular port
- and then plug your computer into that port

ex) tcpdump, tshark

### [2. Sniff all internet traffic is to install a hub between your router and internet modem]

- Hubs work by mirroring al traffic to all prots
- Hubs used to be a common way to build a network
- Have mostly been replaced by more efficient switches


+) You should never consider any network traffic to be secret (112...)

<br/>

# [Network Security]

- These tools include both HW and SW and can protect against a variety of threats

## [1. Firewall]

- Act as a barrier betwen one network and another
- monitor network traffic and allow or block traffic based on a defined set of rules

### 1-1. Software Firewall

- Deny incoming connections unless a rule is set up tp allow it explicitly
- Deny outgoing traffic by default

### 1-2. Hardware Firewall

- They are configured to block any incoming connections that don't match predefined rules

<br/>

# [Application security and safety]

- With C, it is vitalto ensure that allocated memory buffers aren't written past the end
- Whenever you copy data from the network into memory, you must ensure that enough memory was allocated to store the data

<br/>

- Even if you are careful to manage memory perfectly, there are still many more security gotchas to be on the lookout for
- When implementing any network protocol, you should never assume that the data your program receives will adhere to the protocol specification
- These protocol bugs are a concern in any programming language, not just C

<br/>

- If you're implementing a C program intended to run as a server, you should employ a `defense in depth` approach
- Multiple defenses must be overcome before an attacker could cause damage

<br>

1. Writing your program without bugs
2. Don't run your progrm with any more privileges than needed for its functionality
3. Consider not connecting your program directly to the internet (ex) use reverse proxy server)
4. Not wirting networked code in C at all (ex) rewriteen as CGI programs that don't directly interact with the network at all)

<br/>

# [Network-Testing Etiquette]

- Don't test someone else's network witout their explicit permission

