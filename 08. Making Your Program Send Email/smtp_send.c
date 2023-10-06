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

#include "chap08.h"
#include <ctype.h>
#include <stdarg.h>

#define MAXINPUT 512
#define MAXRESPONSE 1024


void get_input(const char *prompt, char *buffer)
{
    printf("%s", prompt);

    buffer[0] = 0;

    // read from stdin
    // does not remove a newline character from the received input;
    // therefore we overwrite the laast character inputted with a terminating null character
    fgets(buffer, MAXINPUT, stdin);

    const int read = strlen(buffer);
    if (read > 0)
        buffer[read-1] = 0;
}


void send_format(SOCKET server, const char *text, ...) {
    char buffer[1024];
    va_list args;

    va_start(args, text);

    // Used to format the text into that buffer
    // Caller to ensure that the formatted output doesn't exceed the reserved buffer space
    vsprintf(buffer, text, args);

    va_end(args);

    send(server, buffer, strlen(buffer), 0);

    printf("C: %s", buffer);
}


/*
STMP response starts with a 3-digit code
Want to parse out this code to check for errors

Each line up to the penultimate line contains a dash character - directly following the 3-digit response code

1. Single line Response
250 Message Received

2. Multi Line Response
250-Message
250 Received!
*/
int parse_response(const char *response) {
    const char *k = response;
    if (!k[0] || !k[1] || !k[2]) return 0;
    for (; k[3]; ++k) {
        if (k == response || k[-1] == '\n') {
            if (isdigit(k[0]) && isdigit(k[1]) && isdigit(k[2])) {
                if (k[3] != '-') {
                    if (strstr(k, "\r\n")) {
                        return strtol(k, 0, 10);
                    }
                }
            }
        }
    }
    return 0;
}

// Waits until a particular response code is received over the network
void wait_on_response(SOCKET server, int expecting) {
    char response[MAXRESPONSE+1];
    char *p = response;
    char *end = response + MAXRESPONSE;

    // not response code haas been received yet
    int code = 0;

    do {
        int bytes_received = recv(server, p, end - p, 0);
        if (bytes_received < 1) {
            fprintf(stderr, "Connection dropped.\n");
            exit(1);
        }

        p += bytes_received;
        *p = 0;

        // We haven't written to the end of the repsonse buffer
        if (p == end) {
            fprintf(stderr, "Server response too large:\n");
            fprintf(stderr, "%s", response);
            exit(1);
        }

        code = parse_response(response);

    } while (code == 0);

    if (code != expecting) {
        fprintf(stderr, "Error from server:\n");
        fprintf(stderr, "%s", response);
        exit(1);
    }

    printf("S: %s", response);
}


/*
- Attempt to open a TCP connection to a given hostname and port number
*/
SOCKET connect_to_host(const char *hostname, const char *port) {
    printf("Configuring remote address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *peer_address;

    // resolve the hostname
    if (getaddrinfo(hostname, port, &hints, &peer_address)) {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }

    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];

    // print the server IP address
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
            address_buffer, sizeof(address_buffer),
            service_buffer, sizeof(service_buffer),
            NI_NUMERICHOST);
    printf("%s %s\n", address_buffer, service_buffer);

    printf("Creating socket...\n");
    SOCKET server;
    server = socket(peer_address->ai_family,
            peer_address->ai_socktype, peer_address->ai_protocol);
    if (!ISVALIDSOCKET(server)) {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }

    printf("Connecting...\n");
    if (connect(server,
                peer_address->ai_addr, peer_address->ai_addrlen)) {
        fprintf(stderr, "connect() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }

    // free memory allocation!!
    freeaddrinfo(peer_address);

    printf("Connected.\n\n");

    return server;
}



int main() {

#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif


    char hostname[MAXINPUT];
    get_input("mail server: ", hostname);

    printf("Connecting to host: %s:25\n", hostname);

    SOCKET server = connect_to_host(hostname, "25");
    wait_on_response(server, 220);

    // Line ending used by SMTP is a carriage return character followed by a line feed character
    send_format(server, "HELO HONPWC\r\n");
    wait_on_response(server, 250);


    // [Important] SMTP defines a specific format for the exchange of email messages and related information
    // To make that format execute below code
    char sender[MAXINPUT];
    get_input("from: ", sender);
    send_format(server, "MAIL FROM:<%s>\r\n", sender);
    wait_on_response(server, 250);

    char recipient[MAXINPUT];
    get_input("to: ", recipient);
    send_format(server, "RCPT TO:<%s>\r\n", recipient);
    wait_on_response(server, 250);

    send_format(server, "DATA\r\n");
    wait_on_response(server, 354);

    char subject[MAXINPUT];
    get_input("subject: ", subject);



    send_format(server, "From:<%s>\r\n", sender);
    send_format(server, "To:<%s>\r\n", recipient);
    send_format(server, "Subject:%s\r\n", subject);


    time_t timer;
    time(&timer);

    struct tm *timeinfo;
    timeinfo = gmtime(&timer);

    char date[128];
    strftime(date, 128, "%a, %d %b %Y %H:%M:%S +0000", timeinfo);

    send_format(server, "Date:%s\r\n", date);

    send_format(server, "\r\n");

    printf("Enter your email text, end with \".\" on a line by itself.\n");

    while (1) {
        char body[MAXINPUT];
        get_input("> ", body);
        send_format(server, "%s\r\n", body);
        if (strcmp(body, ".") == 0) {
            break;
        }
    }


    wait_on_response(server, 250);

    send_format(server, "QUIT\r\n");
    wait_on_response(server, 221);

    printf("\nClosing socket...\n");
    CLOSESOCKET(server);

#if defined(_WIN32)
    WSACleanup();
#endif

    printf("Finished.\n");
    return 0;
}

