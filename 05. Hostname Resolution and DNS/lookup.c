/*
 * MIT License
 *
 * Copyright (c) 2018 Lewis Van Winkle
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "chap05.h"

#ifndef AI_ALL
#define AI_ALL 0x0100
#endif

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage:\n\tlookup hostname\n");
        printf("Example:\n\tlookup example.com\n");
        exit(0);
    }

#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif

    printf("Resolving hostname '%s'\n", argv[1]);
    /*
    - ai_family : address family (ex) AF_INET = IPv4, AP_INET6 = IPv6 etc)
    - ai_socktype : SOCK_STREAM = TCP, SOCK_DGRAM = UPD, 0 : address could be used for either
    - ai_protocol : Should be left to 0 (TCP isn't the only streaming protocol supported by the socket interface etc..) 
    - ai_flags : additional options about how getaddrinfo should work
    */
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));

    // We want both IPv4, IPv6 addressses returned
    hints.ai_flags = AI_ALL;

    // return address list
    struct addrinfo *peer_address;

    /*
    - node (argv[1]) : Specifies a hostname or address as a string
    - service (0) : service or port number as a string
    */
    if (getaddrinfo(argv[1], 0, &hints, &peer_address)) {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }


    printf("Remote address is:\n");
    struct addrinfo *address = peer_address;
    do {
        char address_buffer[100];
        /*
        - convert an addrinfo structure back into a text format
        - It works with both IPv4, IPv6
        - Attempts to perform a "reverse DNS lookup"
            - DNS query : Which IP address does this hostname point to
            - Reverse DNS query : Which hostname does this IP address point to
        */
        getnameinfo(address->ai_addr, address->ai_addrlen,
                address_buffer, sizeof(address_buffer),
                0, 0,
                NI_NUMERICHOST);
        printf("\t%s\n", address_buffer);
    } while ((address = address->ai_next));


    freeaddrinfo(peer_address);

#if defined(_WIN32)
    WSACleanup();
#endif

    return 0;
}

