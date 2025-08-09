#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <openssl/sha.h>
#include <time.h>

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

void generate_websocket_key(char* key) {
    unsigned char random_bytes[16];
    for (int i = 0; i < 16; i++) {
        random_bytes[i] = rand() % 256;
    }
    char* base64_key = base64_encode(random_bytes, 16);
    strcpy(key, base64_key);
    free(base64_key);
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char websocket_key[25] = {0};

    srand(time(NULL));

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Generate WebSocket key
    generate_websocket_key(websocket_key);

    // Construct and send WebSocket upgrade request
    char upgrade_request[512];
    snprintf(upgrade_request, sizeof(upgrade_request),
             "GET / HTTP/1.1\r\n"
             "Host: localhost:%d\r\n"
             "Upgrade: websocket\r\n"
             "Connection: Upgrade\r\n"
             "Sec-WebSocket-Key: %s\r\n" // 이 http header를 추가해야 이 키를 보고 그냥 tcp 통신에서 websocket protocol로 업그레이드 되나봄. 
             "Sec-WebSocket-Version: 13\r\n\r\n",
             PORT, websocket_key);

    send(sock, upgrade_request, strlen(upgrade_request), 0);
    printf("Sent WebSocket upgrade request\n");

    // Receive server response
    int bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
    buffer[bytes_read] = '\0';
    printf("Received server response:\n%s\n", buffer);

    if (strstr(buffer, "101 Switching Protocols") == NULL) {
        printf("WebSocket upgrade failed\n");
        close(sock);
        return -1;
    }

    printf("WebSocket connection established\n");

    // Send a test message
    const char* message = "Hello, WebSocket Server!";
    send(sock, message, strlen(message), 0);
    printf("Sent message: %s\n", message);

    // Receive echo response
    bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
    buffer[bytes_read] = '\0';
    printf("Received echo: %s\n", buffer);

    close(sock);
    return 0;
}
