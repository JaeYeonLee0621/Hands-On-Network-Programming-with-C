# [What is the IoT]

- All connected to the internet
- IoT devics are central to Industry 4.0, commonly considered the fourth industrial revolution

# [Connectivity options]

## [WiFi]

- `ISP (Internet Service Provider)` typically use modems with built in Wi-Fi
- The most popular `Wireless Local Area Network (WLAN)` technology and is based around the IEEE 802.11 standards
- Typically operates on either a 2.4 GHZ or a 5GHZ radio frequency

<br/>

**[1. ad hoc mode]**

- device communicate directly to one another

**[2. infrastructure mode]**

- much more common
- devices on a WLAN all connect to a `Wireless Access Point (WAP)`
- The WAP then generally provides internet access for the whole WLAN

<br/>

- For IoT devices without large screens, getting the network password configured can somtimes be complicated


1. Have your end users put the configuration information on an SD card or USB storage device
2. First connect the device to a computer or smartphone for the initial configuration
3. IoT device intially provide its own Wi-Fi network
- The device acts as a temporary access point
- Laptop or smartphone to connect directly to the device

<br/>

# [Ethernet]

- Wired Ethernet connection is ideal for any IoT device
- Physical cable is required

<br/>

# [Cellular]

1. cellular service is availale nearly everywhere
2. all network setup can be completed at the time of manufacture

<br/> 

1. higher cost (must pay the bill for each specific device)
2. IoT device must contain a cellular modem
3. Certain testing and certification is required before a device is allowed on operator networks

<br/>

# [Bluetooth]

- IEEE 802.15.4-based wireless network
- **WPAN (Wireless Personal Area Networks)**
- direct internet access, but this is uncommon in practice
- This works very wel for the smart watch, as it is usually expected to be in close proximity to a smartphone

<br/>

# [IEEE 802.15.4. WPANs]

- low-rate wireless personal are netowkrs
- useful for low-power, limited-range applications
- Typical ranges extend to about 10 meters with a transfer rate of 250Kbps
- allow device to communicate using IP directly, similar to how Wi-Fi is typically used
- Other protocols are commonly used only to send data to a central hub device
- This hub is the responsible for processing IoT device data and communicating over the internet if needed

<br/>

**[Mesh network topology]**

- common IEEE 802.15.4-based setups
- alows devices remote from the `hub` to communicate through closer devices

<br/>

# [Hardware choices]

## [Single-board computer (SBC)]

- complete computers built on a single circuit board
- Centeral Processing Unit, Random Access memory, non-volatile sotrage, input/output ports
- capable of running full-featured OS

ex) Raspberry Pi Zero W

<br/>

## [MicroController]

- Small computer contained on a single integrated circuit
- All the parts needed for a functional computer, CPU, RAM and non-volatile sotrage

+) SBCs processeor speed : 100~1000 MHz
Microcontroller <= 100 MHz

- IoT device core functionality
- Programmed directly, without an OS

<br/>

# FPGAs (Field-Programmable Gate Arrays)

- Integrated circuits containing arrays of programmable logic blocks
- logic blocks : programmed and combined to implement complex functions
- **Application-Specific Integrated Circuit (ASIC)** can perform can also be done one an FPGA

<br/>

- Useful for highly demanding processing tasks requiring rela-time guarantees (ex) highly prarllel processing)

<br/>

- Programmed using `Hardware description Languages (HDL)` (ex) Verilog, VHSIC Hardware Description Language (VHDL))

<br/>

## [External transceivers and modems]

- modems are available for Wi-Fi, cellular, Ethernet, Bluetooth etc
- A cellular modem is located on the top circuit board, which plugs into a socket on the lower board

<br/>

- External modems often communicate with their host through asynchronous serial

<br/>

# [IoT Protocol]

- few central servers - the cloud
- There is never direct communication from the smartphone to the IoT device

<br/>

- Constrained Application Protocol (CoAP), Message Queuing Telemetry Transport (MQTT)
- Custom designed TCP and UDP protocols are also common

<br/>

# [Firmware Updates]

- If your product uses a microcontroller or FPGA, your device will need to download a firmware image and then transfer that into non-volatile memory as required

<br/>

- You should consider allowing your device's owner to install unauthenticated firmware
- It is not technically possible to retain control of a device that's not physically under your control

ex) if you attempt to prevent a user from installing their own firmware through **Digital Rights Management (DRM)**, you should expect your efforts to be eventually defeated

**+) Digital Rights Management (DRM)**

- Techniques and protocols used to protect digital content from unauthorized access etc

<br/>

- **Using asymmetric Key pair, encrypt symmetric key** : ecrypt using the content distirbutor's **private key**
- Client obtains a DRM license from the content distributor or provider
- **DRM license contains an ecrypted symmetric key**, which is require to decrypt the content
- Built in DRM software or a DRM module to handle the decryption process (using **public key**)
- DRM Provider give public key and DRM license

<br/>

**+) Pallycon : Token License Issuance Flow**

1. Request License Token to the Server
2. Server generate License Token and Sends to Client
3. Client Requests DRM License with License Token to PallyCon server
4. Pallycon server validates license token
5. client encrypts DRM license with public key
6. Client decrpyt content
- client can access the necessary cryptographic keys, including the symmetric key for content decryption

<br/>

- If a redundant firmware copy was stored, then the device can be rolled back to its original factory state ith some sort of physical reset mechanism

<br/>

# [Ethics of IoT]

- Privacy of data collection
- End-of-life planning : Take into consideration if you can keep it useful even after your company discontiues support for it

## Security

- **[Open Web application Security Project (OWAP)](https://www.reflectiz.com/blog/owasp-top-ten-2023/)**
- **Secure Hypertext Internet of Things** : IoT devices should use HTTPS to provide a basic level of security
- TLS implementation designed for embedded use (ex) wolfSSL)
- wolfSSL : also supports DTLS for securing UDP-based protocols

<br/>

- Minimizing bandwidth : IoT device with limited bandwidth will find TLS and DTLS too heavyweight

<br/>

- your cellular providers is able to tunnel your traffic over a secure VPN, without metering the VPN overhead

<br/>

- **Pre-Shared Key (PSK)** : Assign each device a unique encryption key at the time of manufacture
- Symmetric ciphers, avoiding much of the overhead required by TLS, DTLS etc

+) **Message Authentication Code (MAC)**

- Symmetric key algorithms
- Ensure message integrity and authenticity by providing a way to detect whether the message has been tampered with during transmission
- fixed-length string of bits, generated by applying a secret key and a specific algorithm to the message

ex) IPsec, SSL/TLS

**[Procedure]**

1. MAC Generation
2. MAC appended to message : sender appends the generated MAC to the message and sends both the message and the MAC to the receiver
3. MAC verification