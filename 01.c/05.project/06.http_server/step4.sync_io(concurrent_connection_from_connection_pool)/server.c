#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/event.h>  //#include <sys/epoll.h> 대신 mac에선 sys/event.h 사용한다. 
/*
// Linux 구조체 from sys/epoll.h
struct epoll_event {
    uint32_t events;
    epoll_data_t data;
};

// macOS 구조체 from sys.event.h
struct kevent {
    uintptr_t ident;
    int16_t filter;
    uint16_t flags;
    uint32_t fflags;
    intptr_t data;
    void *udata;
};

// Linux (epoll)
epoll_create1(0);
epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

// macOS (kqueue)
kqueue();
EV_SET(&change_event, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
kevent(kq, &change_event, 1, NULL, 0, NULL);
kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
*/
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define BACKLOG 10
#define MAX_EVENTS 64
#define MAX_THREADS 20
#define DOCUMENT_ROOT "./public"

// HTTP Status Lines
#define HTTP_200 "HTTP/1.1 200 OK\r\n"
#define HTTP_404 "HTTP/1.1 404 Not Found\r\n"
#define HTTP_500 "HTTP/1.1 500 Internal Server Error\r\n"

// Thread pool data structures
typedef struct {
    void (*task)(int);
    int arg;
} task_t;

typedef struct {
    task_t* tasks;
    int front;
    int rear;
    int size;
    int capacity;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} task_queue_t;

typedef struct {
    pthread_t* threads;
    int num_threads;
    task_queue_t* queue;
    int stop;
} thread_pool_t;

// Global thread pool
thread_pool_t* thread_pool = NULL;

// Function declarations
void handle_client(int client_fd);
void* worker_thread(void* arg);
int make_socket_non_blocking(int fd);

// Initialize task queue
task_queue_t* init_task_queue(int capacity) {
    task_queue_t* queue = (task_queue_t*)malloc(sizeof(task_queue_t));
    queue->tasks = (task_t*)malloc(capacity * sizeof(task_t));
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    queue->capacity = capacity;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->not_empty, NULL);
    pthread_cond_init(&queue->not_full, NULL);
    return queue;
}

// Initialize thread pool
thread_pool_t* init_thread_pool(int num_threads) {
    thread_pool_t* pool = (thread_pool_t*)malloc(sizeof(thread_pool_t));
    pool->threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    pool->num_threads = num_threads;
    pool->queue = init_task_queue(num_threads * 2);
    pool->stop = 0;

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&pool->threads[i], NULL, worker_thread, pool);
    }
    
    return pool;
}

// Add task to queue
void add_task(thread_pool_t* pool, void (*task)(int), int arg) {
    task_queue_t* queue = pool->queue;
    pthread_mutex_lock(&queue->mutex);
    
    while (queue->size == queue->capacity) {
        pthread_cond_wait(&queue->not_full, &queue->mutex);
    }
    
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->tasks[queue->rear].task = task;
    queue->tasks[queue->rear].arg = arg;
    queue->size++;
    
    pthread_cond_signal(&queue->not_empty);
    pthread_mutex_unlock(&queue->mutex);
}

// Get task from queue
task_t get_task(task_queue_t* queue) {
    pthread_mutex_lock(&queue->mutex);
    
    while (queue->size == 0) {
        pthread_cond_wait(&queue->not_empty, &queue->mutex);
    }
    
    task_t task = queue->tasks[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    
    pthread_cond_signal(&queue->not_full);
    pthread_mutex_unlock(&queue->mutex);
    
    return task;
}

// Worker thread function
void* worker_thread(void* arg) {
    thread_pool_t* pool = (thread_pool_t*)arg;
    
    while (!pool->stop) {
        task_t task = get_task(pool->queue);
        task.task(task.arg);
        close(task.arg);
    }
    
    return NULL;
}

// Make socket non-blocking
int make_socket_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        return -1;
    }
    
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL O_NONBLOCK");
        return -1;
    }
    
    return 0;
}

// Send HTTP response
void send_response(int client_fd, const char* status, const char* content_type, const char* content) {
    char response[BUFFER_SIZE];
    size_t content_len = strlen(content);
    
    snprintf(response, BUFFER_SIZE,
             "%s"
             "Content-Type: %s\r\n"
             "Content-Length: %zu\r\n"
             "\r\n"
             "%s",
             status, content_type, content_len, content);
    
    send(client_fd, response, strlen(response), 0);
}

// Handle client connection
void handle_client(int client_fd) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    
    if (bytes_read <= 0) {
        return;
    }
    
    buffer[bytes_read] = '\0';
    
    const char* content = "<!DOCTYPE html><html><body><h1>Hello from macOS Concurrent Server!</h1></body></html>";
    send_response(client_fd, HTTP_200, "text/html", content);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    // Initialize thread pool
    thread_pool = init_thread_pool(MAX_THREADS);
    
    // Create and setup server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    
    // Make server socket non-blocking
    if (make_socket_non_blocking(server_fd) == -1) {
        exit(EXIT_FAILURE);
    }
    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, BACKLOG) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Create kqueue instance
    int kq = kqueue();
    if (kq == -1) {
        perror("kqueue failed");
        exit(EXIT_FAILURE);
    }

    // Register server socket with kqueue
    struct kevent change_event;
    EV_SET(&change_event, server_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
    
    if (kevent(kq, &change_event, 1, NULL, 0, NULL) == -1) {
        perror("kevent register failed");
        exit(EXIT_FAILURE);
    }

    printf("Server running on port %d\n", PORT);
    
    // Event loop using kqueue
    struct kevent events[MAX_EVENTS];
    
    while (1) {
        int new_events = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
        
        if (new_events == -1) {
            perror("kevent wait failed");
            continue;
        }
        
        for (int i = 0; i < new_events; i++) {
            int event_fd = events[i].ident;
            
            if (event_fd == server_fd) {
                // Handle new connection
                while (1) {
                    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
                    if (client_fd == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            break;
                        }
                        perror("accept failed");
                        break;
                    }
                    
                    // Make client socket non-blocking
                    if (make_socket_non_blocking(client_fd) == -1) {
                        close(client_fd);
                        continue;
                    }
                    
                    // Add task to thread pool
                    add_task(thread_pool, handle_client, client_fd);
                }
            }
        }
    }
    
    // Cleanup (never reached in this example)
    close(server_fd);
    close(kq);
    return 0;
}
