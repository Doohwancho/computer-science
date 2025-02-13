// 기본 standard libraries
#include <stdio.h> //printf(), error(), perror()
#include <stdlib.h> // exit(1);
#include <string.h> //stringlen(), memset()
#include <unistd.h> //file CRUD lib ex. read(), write(), close()

// socket related libs
#include <sys/socket.h> //socket programming. 
#include <netinet/in.h> //Ipv4 internet address 관련 library. ip/port number related
#include <arpa/inet.h> //convert ip address string to binary 
#include <errno.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define BACKLOG 10

// HTTP Response Status Lines
#define HTTP_200 "HTTP/1.1 200 OK\r\n"
#define HTTP_404 "HTTP/1.1 404 Not Found\r\n"
#define HTTP_500 "HTTP/1.1 500 Internal Server Error\r\n"

// HTTP Response Headers
#define CONTENT_TYPE "Content-Type: text/html\r\n"
#define CONTENT_LENGTH "Content-Length: %zu\r\n"
#define NEWLINE "\r\n"

// Simple HTML responses
const char* response_200 = "<!DOCTYPE html><html><body><h1>200 OK</h1></body></html>";
const char* response_404 = "<!DOCTYPE html><html><body><h1>404 Not Found</h1></body></html>";
const char* response_500 = "<!DOCTYPE html><html><body><h1>500 Internal Server Error</h1></body></html>";

typedef struct {
    char method[16];      // GET, POST, etc.
    char path[2048];      // /index.html, /api/data, etc.
    char version[16];     // HTTP/1.1
    char host[1024];      // Host header value
    size_t content_length; // Content-Length header value
} http_request;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

// Parse HTTP request headers
void parse_http_request(char* buffer, http_request* req) {
    // Initialize request struct
    memset(req, 0, sizeof(http_request));
    
    // Parse first line (GET /path HTTP/1.1)
    char* line = strtok(buffer, "\r\n");
    if (line) {
        sscanf(line, "%s %s %s", req->method, req->path, req->version);
    }
    
    // Parse headers
    while ((line = strtok(NULL, "\r\n")) != NULL && strlen(line) > 0) {
        if (strncmp(line, "Host: ", 6) == 0) {
            strcpy(req->host, line + 6);
        }
        else if (strncmp(line, "Content-Length: ", 16) == 0) {
            req->content_length = atoi(line + 16);
        }
    }
}

// Send HTTP response
void send_response(int client_fd, const char* status, const char* content) {
    char response[BUFFER_SIZE];
    size_t content_len = strlen(content);
    
    // Build response with headers
    snprintf(response, BUFFER_SIZE,
             "%s"           // Status line
             "%s"           // Content-Type
             CONTENT_LENGTH // Content-Length
             NEWLINE       // Empty line
             "%s",         // Content
             status,
             CONTENT_TYPE,
             content_len,
             content);
    
    // Send response
    send(client_fd, response, strlen(response), 0);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};
    
    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        error("Failed to create socket");
    }
    
    // Set socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        error("setsockopt failed");
    }
    
    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        error("Bind failed");
    }
    
    // Listen for connections
    if (listen(server_fd, BACKLOG) == -1) {
        error("Listen failed");
    }
    printf("HTTP Server listening on port %d\n", PORT);
    
    // Main server loop
    while (1) {
        // Accept connection
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd == -1) {
            perror("Accept failed");
            continue;
        }
        
        // Read request
        ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
        if (bytes_read <= 0) {
            close(client_fd);
            continue;
        }
        buffer[bytes_read] = '\0';
        
        // Parse HTTP request
        http_request req;
        parse_http_request(buffer, &req);
        
        // Log request
        printf("\nReceived request:\nMethod: %s\nPath: %s\nVersion: %s\nHost: %s\n",
               req.method, req.path, req.version, req.host);
        
        // Handle request (only GET for now)
        if (strcmp(req.method, "GET") == 0) {
            if (strcmp(req.path, "/") == 0 || strcmp(req.path, "/index.html") == 0) {
                send_response(client_fd, HTTP_200, response_200);
            } else {
                send_response(client_fd, HTTP_404, response_404);
            }
        } else {
            // Method not supported
            send_response(client_fd, HTTP_500, response_500);
        }
        
        // Close connection
        close(client_fd);
    }
    
    // Close server socket
    close(server_fd);
    return 0;
}
