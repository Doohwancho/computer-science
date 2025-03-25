// standard lib
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// file related 
#include <unistd.h> //read(), write(), etc
#include <fcntl.h> //file descriptor (read, write flag) 

// socket related 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// 비동기적 처리 
#include <pthread.h>
#include <sys/event.h>
#include <sys/uio.h>
#include <sys/types.h>

// util 
#include <sys/time.h>
#include <errno.h>


#define PORT 8080
#define BUFFER_SIZE 4096
#define MAX_EVENTS 64
#define MAX_WORKERS 4

// Operation types (similar to IOCP)
typedef enum {
    OP_ACCEPT,
    OP_READ,
    OP_WRITE
} operation_type;

// Overlapped structure (similar to IOCP OVERLAPPED)
typedef struct {
    operation_type op_type;
    void* buffer;
    size_t buffer_len;
    int socket_fd;
    void (*callback)(struct kevent*);
} overlapped_t;

// Completion Port structure
typedef struct {
    int kqueue_fd;
    pthread_t worker_threads[MAX_WORKERS];
    int running;
} completion_port_t;

// Global variables
completion_port_t* g_iocp = NULL;
int g_server_fd;

// Function declarations
void handle_accept(struct kevent* event);
void handle_read(struct kevent* event);
void handle_write(struct kevent* event);
void* worker_thread(void* arg);

// Initialize completion port
completion_port_t* create_completion_port() {
    completion_port_t* iocp = malloc(sizeof(completion_port_t));
    iocp->kqueue_fd = kqueue();
    if (iocp->kqueue_fd == -1) {
        free(iocp);
        return NULL;
    }
    iocp->running = 1;
    return iocp;
}

// Post async operation (similar to PostQueuedCompletionStatus)
void post_async_operation(overlapped_t* overlapped) {
    struct kevent event;
    
    switch (overlapped->op_type) {
        case OP_READ:
            EV_SET(&event, overlapped->socket_fd, EVFILT_READ, 
                   EV_ADD | EV_ENABLE | EV_ONESHOT, 
                   0, 0, overlapped);
            break;
            
        case OP_WRITE:
            EV_SET(&event, overlapped->socket_fd, EVFILT_WRITE, 
                   EV_ADD | EV_ENABLE | EV_ONESHOT, 
                   0, 0, overlapped);
            break;
            
        default:
            return;
    }
    
    kevent(g_iocp->kqueue_fd, &event, 1, NULL, 0, NULL);
}

// Start async accept
void post_accept() {
    overlapped_t* overlapped = malloc(sizeof(overlapped_t));
    overlapped->op_type = OP_ACCEPT;
    overlapped->socket_fd = g_server_fd;
    overlapped->callback = handle_accept;
    
    struct kevent event;
    EV_SET(&event, g_server_fd, EVFILT_READ, 
           EV_ADD | EV_ENABLE, 0, 0, overlapped);
    kevent(g_iocp->kqueue_fd, &event, 1, NULL, 0, NULL);
}

// Start async read
void post_read(int socket_fd) {
    overlapped_t* overlapped = malloc(sizeof(overlapped_t));
    overlapped->op_type = OP_READ;
    overlapped->socket_fd = socket_fd;
    overlapped->buffer = malloc(BUFFER_SIZE);
    overlapped->buffer_len = BUFFER_SIZE;
    overlapped->callback = handle_read; //async 비동기 처리는 필드에 ->callback 추가해서 함수를 넘겨주는구나 
    
    post_async_operation(overlapped);
}

// Start async write
void post_write(int socket_fd, const char* response, size_t len) {
    overlapped_t* overlapped = malloc(sizeof(overlapped_t));
    overlapped->op_type = OP_WRITE;
    overlapped->socket_fd = socket_fd;
    overlapped->buffer = malloc(len);
    memcpy(overlapped->buffer, response, len);
    overlapped->buffer_len = len;
    overlapped->callback = handle_write;
    
    post_async_operation(overlapped);
}

// Handle completed accept
void handle_accept(struct kevent* event) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    int client_fd = accept(g_server_fd, 
                          (struct sockaddr*)&client_addr, 
                          &client_len);
    
    if (client_fd != -1) {
        // Make client socket non-blocking
        int flags = fcntl(client_fd, F_GETFL, 0);
        fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
        
        // Start reading from client
        post_read(client_fd);
    }
    
    // Continue accepting
    post_accept();
}

// Handle completed read
void handle_read(struct kevent* event) {
    overlapped_t* overlapped = (overlapped_t*)event->udata;
    int client_fd = overlapped->socket_fd;
    
    if (event->data > 0) {  // Data available
        // Prepare HTTP response
        const char* response = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "Hello, World!";
        
        // Start async write
        post_write(client_fd, response, strlen(response));
    } else {
        close(client_fd);
    }
    
    free(overlapped->buffer);
    free(overlapped);
}

// Handle completed write
void handle_write(struct kevent* event) {
    overlapped_t* overlapped = (overlapped_t*)event->udata;
    close(overlapped->socket_fd);
    free(overlapped->buffer);
    free(overlapped);
}

// Worker thread function
void* worker_thread(void* arg) {
    struct kevent events[MAX_EVENTS];
    
    while (g_iocp->running) {
        int nev = kevent(g_iocp->kqueue_fd, NULL, 0, 
                        events, MAX_EVENTS, NULL);
        
        for (int i = 0; i < nev; i++) {
            overlapped_t* overlapped = events[i].udata;
            if (overlapped && overlapped->callback) {
                overlapped->callback(&events[i]);
            }
        }
    }
    
    return NULL;
}

int main() {
    // Create server socket
    g_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (g_server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Set socket options
    int opt = 1;
    setsockopt(g_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // Make server socket non-blocking
    int flags = fcntl(g_server_fd, F_GETFL, 0);
    fcntl(g_server_fd, F_SETFL, flags | O_NONBLOCK);
    
    // Bind and listen
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if (bind(g_server_fd, (struct sockaddr*)&server_addr, 
             sizeof(server_addr)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(g_server_fd, SOMAXCONN) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    
    // Initialize completion port
    g_iocp = create_completion_port();
    if (!g_iocp) {
        perror("create_completion_port failed");
        exit(EXIT_FAILURE);
    }
    
    // Start worker threads
    for (int i = 0; i < MAX_WORKERS; i++) {
        pthread_create(&g_iocp->worker_threads[i], NULL, 
                      worker_thread, NULL);
    }
    
    // Start accepting connections (accept http_request from clients)
    post_accept();
    
    printf("Server running on port %d\n", PORT);
    
    // Wait for worker threads
    for (int i = 0; i < MAX_WORKERS; i++) {
        pthread_join(g_iocp->worker_threads[i], NULL);
    }
    
    return 0;
}
