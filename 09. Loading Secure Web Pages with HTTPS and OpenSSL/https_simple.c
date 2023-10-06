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

#include "chap09.h"


int main(int argc, char *argv[]) {

#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif

    // Required to initialized the OpenSSL library
    SSL_library_init();

    // OpenSSL to load all available algorithms
    OpenSSL_add_all_algorithms();

    // cause OpenSSL to load error strings
    SSL_load_error_strings();

    /*
    SSL_CTX
    - Hold the initial settings that you want to use for your connections
    - Only one SSL_CTX object and can reuse it for all their connections

    TLS_client_method
    - indicates that we want general-purpose, version-flexible TLS methods available
    - automatically negotitates the best mutually supported algorithm with the server upon connecting
    */
    
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        fprintf(stderr, "SSL_CTX_new() failed.\n");
        return 1;
    }

    if (argc < 3) {
        fprintf(stderr, "usage: https_simple hostname port\n");
        return 1;
    }

    char *hostname = argv[1];
    char *port = argv[2];


    printf("Configuring remote address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *peer_address;
    if (getaddrinfo(hostname, port, &hints, &peer_address)) {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        exit(1);
    }

    printf("Remote address is: ");
    char address_buffer[100];
    char service_buffer[100];
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
    freeaddrinfo(peer_address);

    printf("Connected.\n\n");

    // Once TCP connection is established you can initiate a TLS connection
    SSL *ssl = SSL_new(ctx);
    if (!ssl) {
        fprintf(stderr, "SSL_new() failed.\n");
        return 1;
    }

    // Open SSL to use SNI
    // without it server does not know which certificate to send in the case that it hosts more than one site
    if (!SSL_set_tlsext_host_name(ssl, hostname)) {
        fprintf(stderr, "SSL_set_tlsext_host_name() failed.\n");
        ERR_print_errors_fp(stderr);
        return 1;
    }

    // initiate the new TLS/SSL connection on our existing TCP socket
    SSL_set_fd(ssl, server);
    if (SSL_connect(ssl) == -1) {
        fprintf(stderr, "SSL_connect() failed.\n");
        ERR_print_errors_fp(stderr);
        return 1;
    }

    printf ("SSL/TLS using %s\n", SSL_get_cipher(ssl));


    /* 
    get the server's certificate

    1. Verify Certificate
    2. Check which certificate authorities we trust

    - OS provides a list of trusted certificates
    - But there is not general, easy way to import these lists
    - Using the OS's default list is also not always appropriate for every application
    - For that reasons, certificate veritifcation has been omitted from the examples in this chapter

    - It is also important to validate that the certificate is actually valid for the particular server you're connected with
    */
    X509 *cert = SSL_get_peer_certificate(ssl);
    if (!cert) {
        fprintf(stderr, "SSL_get_peer_certificate() failed.\n");
        return 1;
    }

    char *tmp;
    if ((tmp = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0))) {
        printf("subject: %s\n", tmp);
        OPENSSL_free(tmp);
    }

    if ((tmp = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0))) {
        printf("issuer: %s\n", tmp);
        OPENSSL_free(tmp);
    }

    X509_free(cert);


    char buffer[2048];

    sprintf(buffer, "GET / HTTP/1.1\r\n");
    sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", hostname, port);
    sprintf(buffer + strlen(buffer), "Connection: close\r\n");
    sprintf(buffer + strlen(buffer), "User-Agent: https_simple\r\n");
    sprintf(buffer + strlen(buffer), "\r\n");

    SSL_write(ssl, buffer, strlen(buffer));
    printf("Sent Headers:\n%s", buffer);

    while(1) {
        int bytes_received = SSL_read(ssl, buffer, sizeof(buffer));
            if (bytes_received < 1) {
                printf("\nConnection closed by peer.\n");
                break;
            }

            printf("Received (%d bytes): '%.*s'\n",
                    bytes_received, bytes_received, buffer);

    } //end while(1)

    printf("\nClosing socket...\n");
    SSL_shutdown(ssl);
    CLOSESOCKET(server);
    SSL_free(ssl);
    SSL_CTX_free(ctx);

#if defined(_WIN32)
    WSACleanup();
#endif

    printf("Finished.\n");
    return 0;
}

