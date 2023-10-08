# [TCP(Transmission Control Protocol) socket tips]

## [TCP Connection Lifespan]

### The setup phase

- Consider what happens if the target system doesn't response
- By default, connect() sometimes waits a loong time, attempting to establish a TCP connection

### The data-transfer phase
- Must be careful to prevent deadlocks

ex) slient and server call recv immediatley after the connection is established

- The both then wait until their peer acknowledges receipt before sending more

+) Prevention

ex) HTTP : the client sends a request, the server sends a reply

- but TCP is full-duplex protocol

So the solution!!! alternate calls to send() with calls to recv()
> The liberal use of `select()` will help us do this efficiently

### The tear-down phase
- Haven't lost data at the end of a connection

<br/>

## [Timeout on connect()]

- connect() blocks until the connection is established (after 3 handshake)
- connect() blocks for at least one round-trip network time
- The timeout preiod is controlled by the OS
- The exact timeout period varies but 20 seconds is about typical
- But you can always call connect() again if you want to keep trying

<br/>

- A cross-platform connect() timeout can be achieved by using `select()`

**+) select()**

- allows us to wait ona socket operation with a specified timeout
- allowing your proram to do useful work while aiting for the TCP connection to be established
- used to wait on multiple connect() calls and other socket event besides
- It can work well for a client that needs to connect to several servers in parallel

<br/>

## [Congestion Control]

- Congestion control methods as part of a network congestion avoidance
- `TCP Slow start algorithm` : provides a way for TCP to ramp-up a connection to its full potential, instad of immediately dumping a lot of data on the network all at once

### Slow start algorithm

- Cause problems for short-lived connections
- If a connection needs to send only a small amount of data, that connection won't ever reach its full potential
- This has cause many protocols to be designed around keeping connections open for longer

<br/>

# [The Nagle algorithm]

- Make the sender pool small amounts of data together until it has enough to justify sending
- A small segment is considered any segment less than MSS (Maximum Segment Size)
- You should always prefer doing one large write to send() instead of many small ones

<br/>

- In some applications you really do need to send a small packet followed by another small packet immediately after

ex) in a real-time multiplayer video game
- you can't queue up player commands

- Many real-time applications prefer using UDP over TCP

ex) Video game, Video streaming etc

### +) UDP over TCP

- UDP packets are encapsulated within a TCP
- Using TCP as a transport mechanism for UDP traffic

ex)

- Some networks or firewalls may block UDP traffic due to security concerns or policies

<br/>

## Delayed ACK

- Specific TCP optimization mechanism
- If the receiver `has no data to send back to the sender during the waiting period` > it then sends a standalone ACK for thhe received data
- If the receiver `does have data to send in response to the received packet` > it includes the ACK within the payload of its outgoing data packet

### +) piggyback

- Broader concept that can refer to the practice of combining multiple pieces of information or operations in a single transmission

<br/>

- In the context of TCP, often used in the deplayed ACK mechanism
- Practice of `combining an acknowledgement packet with a data packet when sending data over a network`

<br/>

## [Connection tear-down]

- TCP connection = **full-duplex**
- Connection must be closed by both sides before it is truly disconnected

<br/>

1. You initiate the tear-down by sending the first FIN message
2. You receive a FIN message from your connected peer
3. You and your peer and FIN message simultaneously

<br/>

- **One-half closed** : Closing initiator sends a FIN message to its peer, the peer replies with an ACK
- The initiator can no longer send data, but it can still receive data
- The peer is ready to finish closing the connection, it sends its own FIN message
- The initiator then responds with the final ACK message, and the connection is fully closed

<br/>

- The `close()` function closes both sides of a coekt
- If you use `close()` in your application, and the remote peer tries to send more data, it will cause an error
- Your system will then transmit a `Reset(RST)` message to indicate to the peer that the conection was not closed in an orderly manner

+) `shutdown()` : close only their sneding data channel

- if) application protocol doesn't have a way to signal that it has finished sending data
- if) application isn't tolerance of missed data

+) `close()` : server won't be sending additional data

<br/>

## [Preventing address-in-use errors]

- When an application initiates a TCP socket close, that socket goes into the TIME-WAIT state
- OS continues to keep track of this socket for some time, potentially minutes

<br/>

## [Sending to a disconnected peer]

### [3 basic way a TCP connection can fail]

**1. A network outage**
- TCP tries to retransmit data
- If connectivity is re-established, TCP simply picks back up where it left off
- The connection eventually time out, This timeout can be on the order of 10 minutes

**2. The peer application crashes**

- The peer's OS sends a FIN mssage
- If your applicatio continues to send data after having received the FIN message, <u>the peer's system will send a RST message to indicate the error</u>

**3. The peer's system crashes**

- Peer's whole system has crashed
- It won't be able to send a FIN message
- This case looks the same as a network outage, TCP connection would eventually timeout
- The rebooted system will not recognize the TCP connection and will send an RST message in response to indicate an error condition

<br/>

- What happens when `send()` is called on a socket that has already received an RST message from its peer
- the default is to occur a `SIGPIPE` signal to your program
- If you don't handle this signal, the OS will terminate your program

<br/>

**+) RST (Reset)**

- Control message used to abort an ongoing connection or reject an incoming connection request
- Way for one side of a TCP connection to forcefully terminate the connection in response to various situation

<br/>

**+) SIGPIPE**

- Signal: Pipe
- Signal that is typically sent to a process when it attempts to write to a pipe or socket that has been closed by the other end

<br/>

## [Multiplexing with a large number of sockets]

- If you have a large number of open sockets, you can quickly run into the limitations of select()
- The maximum number is available through the FD_SETSIZE macro

<br/>

- Linux defines fd_set as a bitmask, and it is not possible to increase its size without recompiling the kernel
- Using different multiplexing technique : `poll()`