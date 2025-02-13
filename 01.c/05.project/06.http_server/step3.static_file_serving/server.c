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

// static file serving libs
#include <sys/stat.h> //파일의 상태와 정보를 다루는 라이브러리. ex. 파일의 메타데이터, 크기, 마지막 수정 상태 등...
#include <sys/types.h> //시스템 타입 정의. ex. size_t
#include <fcntl.h> //open()으로 파일열 때, 플래그. ex. 읽기전용, 쓰기전용 등..

// helper lib
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define BACKLOG 10
#define MAX_PATH_LENGTH 2048
#define DOCUMENT_ROOT "./public"  // Serve files from ./public directory

// HTTP Status Lines
#define HTTP_200 "HTTP/1.1 200 OK\r\n"
#define HTTP_404 "HTTP/1.1 404 Not Found\r\n"
#define HTTP_500 "HTTP/1.1 500 Internal Server Error\r\n"
#define HTTP_403 "HTTP/1.1 403 Forbidden\r\n"

// HTTP Headers
#define SERVER_HEADER "Server: C Static File Server\r\n"
#define DATE_HEADER "Date: %s\r\n"
#define CONTENT_TYPE "Content-Type: %s\r\n"
#define CONTENT_LENGTH "Content-Length: %zu\r\n"
#define LAST_MODIFIED "Last-Modified: %s\r\n"
#define NEWLINE "\r\n"

typedef struct {
    char method[16];
    char path[MAX_PATH_LENGTH];
    char version[16];
    char host[1024];
    size_t content_length;
} http_request;

// MIME type mapping structure
typedef struct {
    const char* extension;
    const char* mime_type;
} mime_map;

// Common MIME types
mime_map mime_types[] = {
    {".html", "text/html"},
    {".htm", "text/html"},
    {".css", "text/css"},
    {".js", "application/javascript"},
    {".json", "application/json"},
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".png", "image/png"},
    {".gif", "image/gif"},
    {".svg", "image/svg+xml"},
    {".txt", "text/plain"},
    {NULL, "application/octet-stream"}  // Default MIME type
};

void error(const char *msg) {
    perror(msg);
    exit(1);
}

// Get MIME type based on file extension
const char* get_mime_type(const char* filepath) {
    const char* dot = strrchr(filepath, '.');
    if (!dot) return "application/octet-stream";
    
    for (int i = 0; mime_types[i].extension != NULL; i++) {
        if (strcasecmp(dot, mime_types[i].extension) == 0) {
            return mime_types[i].mime_type;
        }
    }
    return "application/octet-stream";
}

// Format current time for HTTP headers
void get_current_time(char* buffer, size_t size) {
    time_t now = time(NULL);
    struct tm* tm_info = gmtime(&now);
    strftime(buffer, size, "%a, %d %b %Y %H:%M:%S GMT", tm_info);
}

// Format file modification time for HTTP headers
void get_file_time(time_t time, char* buffer, size_t size) {
    struct tm* tm_info = gmtime(&time);
    strftime(buffer, size, "%a, %d %b %Y %H:%M:%S GMT", tm_info);
}

// Parse HTTP request
void parse_http_request(char* buffer, http_request* req) {
    memset(req, 0, sizeof(http_request));
    
    char* line = strtok(buffer, "\r\n");
    if (line) {
        sscanf(line, "%s %s %s", req->method, req->path, req->version);
    }
    
    while ((line = strtok(NULL, "\r\n")) != NULL && strlen(line) > 0) {
        if (strncmp(line, "Host: ", 6) == 0) {
            strcpy(req->host, line + 6);
        }
        else if (strncmp(line, "Content-Length: ", 16) == 0) {
            req->content_length = atoi(line + 16);
        }
    }
}

// Send HTTP headers
void send_headers(int client_fd, const char* status, const char* mime_type, 
                 size_t content_length, time_t last_modified) {
    char date[100];
    char modified[100];
    get_current_time(date, sizeof(date));
    get_file_time(last_modified, modified, sizeof(modified));
    
    char headers[BUFFER_SIZE];
    snprintf(headers, BUFFER_SIZE,
             "%s"           // Status line
             "%s"           // Server header
             "Date: %s\r\n" // Date header
             "Content-Type: %s\r\n"
             "Content-Length: %zu\r\n"
             "Last-Modified: %s\r\n"
             "\r\n",        // Empty line
             status,
             SERVER_HEADER,
             date,
             mime_type,
             content_length,
             modified);
    
    send(client_fd, headers, strlen(headers), 0);
}

// Send error response
void send_error_response(int client_fd, const char* status, const char* message) {
    char html[BUFFER_SIZE];
    snprintf(html, BUFFER_SIZE,
             "<!DOCTYPE html><html><body><h1>%s</h1><p>%s</p></body></html>",
             status, message);
    
    send_headers(client_fd, status, "text/html", strlen(html), time(NULL));
    send(client_fd, html, strlen(html), 0);
}

// Serve static file
void serve_file(int client_fd, const char* filepath) {
    // Get file information
    struct stat file_stat;
    if (stat(filepath, &file_stat) == -1) {
        send_error_response(client_fd, HTTP_404, "File not found");
        return;
    }
    
    // Check if it's a regular file
    if (!S_ISREG(file_stat.st_mode)) {
        send_error_response(client_fd, HTTP_403, "Not a regular file");
        return;
    }
    
    // Open file
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        send_error_response(client_fd, HTTP_500, "Failed to open file");
        return;
    }
    
    // Get MIME type and send headers
    const char* mime_type = get_mime_type(filepath);
    send_headers(client_fd, HTTP_200, mime_type, file_stat.st_size, file_stat.st_mtime);
    
    // Send file content
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        send(client_fd, buffer, bytes_read, 0);
    }
    
    close(fd);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    
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
    
    // Initialize server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind and listen
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        error("Bind failed");
    }
    if (listen(server_fd, BACKLOG) == -1) {
        error("Listen failed");
    }
    
    printf("File server running on port %d\nServing files from %s\n", PORT, DOCUMENT_ROOT);
    
    // Main loop
    while (1) {
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
        
        // Parse request
        http_request req;
        parse_http_request(buffer, &req);
        
        // Log request
        printf("[%s] %s %s\n", req.method, req.path, req.version);
        
        if (strcmp(req.method, "GET") != 0) {
            send_error_response(client_fd, HTTP_500, "Only GET method is supported");
            close(client_fd);
            continue;
        }
        
        // Construct file path
        char filepath[MAX_PATH_LENGTH];
        snprintf(filepath, sizeof(filepath), "%s%s", DOCUMENT_ROOT, req.path);
        
        // If path ends with '/', append index.html
        if (filepath[strlen(filepath) - 1] == '/') {
            strcat(filepath, "index.html");
        }
        
        // Serve the file
        serve_file(client_fd, filepath);
        
        close(client_fd);
    }
    
    close(server_fd);
    return 0;
}
