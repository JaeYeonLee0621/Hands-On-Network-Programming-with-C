# [How hostname resolution works]

- DNS is used to assign names to computers and systems connected to the internet
- When your program needs to connect to a remote computer, it first need to find the IP address for www.example.com
	 - In this book so far, we have been using the built-in getaddrinfo() function for this purpose
	 - When you call getaddrinfo(), your OS goest hrough a number of steps to resolve the domain

	 1. OS checks whether it already knows the IP address for www.example.com
	 	- OS is allowed to remember it in a local cache for a time
	 	- This time is referred to as TTL (Time To Live)
	 	- It it set by the DNS server repsonsible for the hostname

 	2. (is not found) OS will need to query a DNS server
 		- not only ISP (Internet Service Provider) but also many publicly-available DNS servers
 		- DNS server receives a query, it also checks its local cache

 	3. (if DNS server don't have) Needs to query other DNS servers until it connects directly to the DNS server responsible for the target system

		1. First connects to a root DNS server and asks for www.example.com
		2. The root DNS server directs it to ask the .com server
		3. Our client's DNS server then connects to the server responsible for `.com` and asks for www.example.com
		4. The `.com` DNS server gives our server the address of another server
		5. Our DNS server finally connects to that server and asks about the record for www.example.com
		6. The www.example.com server then shares the address of www.example.com
		7. Our client's DNS server relays it back to our client

# [DNS Record Types]

- Client has 5 main type
- DNS's primary purpose is to translate hostnames into IP addressees

- `A, AAAA` : IPv4, IPv6
- `MX` : mail server information
- `TXT` : store arbitrary information for a hostname
	- Set to prove ownership of a domain name or to publish email sending guidlines
	- Sender Policy Framework (SPF) standard uses TXT records
- `CNAME` : Provide an alias (subdomain) for a given name (ex) example.com, www.example.com)
- `*, ALL, ANY` : Then DNS server returns all known record types for the currrent query
	- DNS server is allowed to respond with only the records in its cache
	- this query is not guaranteed to actually get all of the records for the requested domain

# [DNS Security]

- DNS is still widely used in an unsecured manner
- Porotocl do exist to provide security for DNS, buty they are not widely adopted yet

## Domain Name System Security Extensions (DNSSEC)
- provide data authentication

## DNS over HTTPS (DoG)
- Provide name resolution over HTTPS

# [The DNS Protocol]

- When a client wants to resolve a host anme into an IP address, it sends a DNS query to a DNS server
- This typically done over `UDP using port 53`

### [TCP fall back, DNS transport over TCP]

- If the query is too large to fit into one UDP packet, then the query can be performed over TCP instead of UDP

- In this caase, the size of the query is sent over CP as a 16vit-value, and then the query itself is sent

- But UDP works for most cases

<br/>

- The client must know the IP address of at least one DNS server
- If the client doesn't know of any DNS servers, then it has a sort of chicken-and-egg problem
- DNS server are usually provided by your `ISP`

# [Endianness]

- The order in which individual bytes are stored in memory or sent over a network
- Whenever we read a multi-byte number from a DNS message, we should be aware that it's in big-endian format (network byte order)
