#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <libcc.h>

#define PORT 5500
#define BUFFER_SIZE 1024

SSL_CTX *ssl_ctx;

void init_openssl() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    ssl_ctx = SSL_CTX_new(TLS_server_method());
    if (!ssl_ctx) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

void configure_context(const char *cert_file, const char *key_file) {
    if (ssl_ctx) {
        static _cc_atomic64_t session_id = 89877;//time(nullptr);
        uint64_t session_id_context = _cc_atomic64_inc(&session_id);
        SSL_CTX_set_session_id_context(ssl_ctx, (byte_t*)&session_id_context, sizeof(session_id_context));
    }
    if (SSL_CTX_use_certificate_file(ssl_ctx, cert_file, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    if (SSL_CTX_use_PrivateKey_file(ssl_ctx, key_file, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_NONE, NULL); // Disable certificate verification for testing
}

int create_socket() {
    int sockfd;
    struct sockaddr_in addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 10) < 0) {
        perror("Unable to listen");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

void handle_client(int client_fd) {
    SSL *ssl;
    char buffer[BUFFER_SIZE];
    int bytes;

    ssl = SSL_new(ssl_ctx);
    SSL_set_fd(ssl, client_fd);

    if (ssl == NULL) {
        fprintf(stderr, "SSL object is NULL\n");
        return;
    }
    if (SSL_accept(ssl) <= 0) {
        //ERR_print_errors_fp(stderr);
        fprintf(stderr, "SSL handshake failed: %s\n", ERR_error_string(ERR_get_error(), NULL));
        SSL_shutdown(ssl);
        SSL_free(ssl);
        return ;
        
    } else {
        bytes = SSL_read(ssl, buffer, sizeof(buffer));
        if (bytes > 0) {
            printf("Received: %.*s\n", bytes, buffer);
            // Handle WebSocket handshake here
            const char *response = "HTTP/1.1 101 Switching Protocols\r\n"
                                   "Upgrade: websocket\r\n"
                                   "Connection: Upgrade\r\n"
                                   "Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\n\r\n";
            SSL_write(ssl, response, strlen(response));
        }
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(client_fd);
}

int main() {
    int sockfd, client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    init_openssl();
    configure_context("cert.pem", "key.pem");
    sockfd = create_socket();

    printf("Server listening on port %d\n", PORT);

    while (1) {
        client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("Unable to accept");
            continue;
        }

        handle_client(client_fd);
    }

    close(sockfd);
    SSL_CTX_free(ssl_ctx);
    EVP_cleanup();
    return 0;
}