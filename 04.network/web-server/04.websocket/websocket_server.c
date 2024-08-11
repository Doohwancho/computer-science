#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/sha.h>

#define PORT 8080
#define BUFFER_SIZE 1024

char* base64_encode(const unsigned char* input, int length) {
    static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int output_length = 4 * ((length + 2) / 3);
    char* output = (char*)malloc(output_length + 1);
    int i, j;

    for (i = 0, j = 0; i < length;) {
        uint32_t octet_a = i < length ? input[i++] : 0;
        uint32_t octet_b = i < length ? input[i++] : 0;
        uint32_t octet_c = i < length ? input[i++] : 0;

        uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;

        output[j++] = base64_chars[(triple >> 18) & 0x3F];
        output[j++] = base64_chars[(triple >> 12) & 0x3F];
        output[j++] = base64_chars[(triple >> 6) & 0x3F];
        output[j++] = base64_chars[triple & 0x3F];
    }

    for (i = 0; i < (3 - length % 3) % 3; i++)
        output[output_length - 1 - i] = '=';

    output[output_length] = '\0';
    return output;
}

void generate_websocket_key(const char* client_key, char* server_key) {
    char combined_key[128];
    snprintf(combined_key, sizeof(combined_key), "%s258EAFA5-E914-47DA-95CA-C5AB0DC85B11", client_key);

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1((unsigned char*)combined_key, strlen(combined_key), hash);

    char* base64_hash = base64_encode(hash, SHA_DIGEST_LENGTH);
    strcpy(server_key, base64_hash);
    free(base64_hash);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        // Accept client connection
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        // Read client request
        int bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
        if (bytes_read < 0) {
            perror("Read failed");
            close(client_fd);
            continue;
        }
        buffer[bytes_read] = '\0';

        // Check if it's a WebSocket upgrade request
        if (strstr(buffer, "Upgrade: websocket") != NULL) {
            char* sec_websocket_key = strstr(buffer, "Sec-WebSocket-Key: ");
            if (sec_websocket_key) {
                sec_websocket_key += 19;  // Length of "Sec-WebSocket-Key: "
                char* key_end = strchr(sec_websocket_key, '\r');
                if (key_end) {
                    *key_end = '\0';

                    char server_key[29];
                    generate_websocket_key(sec_websocket_key, server_key);

                    // Send WebSocket handshake response
                    char response[256];
                    snprintf(response, sizeof(response),
                             "HTTP/1.1 101 Switching Protocols\r\n"
                             "Upgrade: websocket\r\n"
                             "Connection: Upgrade\r\n"
                             "Sec-WebSocket-Accept: %s\r\n\r\n",
                             server_key);
                    write(client_fd, response, strlen(response));

                    printf("WebSocket connection established\n");

                    // WebSocket communication loop
                    while (1) {
                        bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
                        if (bytes_read <= 0) break;

                        // Simple echo for demonstration (no proper WebSocket frame parsing)
                        write(client_fd, buffer, bytes_read);
                    }
                }
            }
        } else {
            // Regular HTTP response
            const char* http_response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!";
            write(client_fd, http_response, strlen(http_response));
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
