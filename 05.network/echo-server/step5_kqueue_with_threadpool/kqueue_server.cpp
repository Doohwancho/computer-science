#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

/* Include our enhanced event loop library */
/* In a real project this would be in separate files */
#include "my_kqueue.h"
#include "threadpool.h"

#define SERVER_PORT 8080
#define BACKLOG 128
#define MAX_CLIENTS 1024
#define BUFFER_SIZE 4096
#define THREAD_POOL_SIZE 4

/* Define task types */
typedef enum {
    ROUTE_LIGHT,      /* Quick operation - process in event loop */
    ROUTE_HEAVY       /* Heavy operation - offload to thread pool */
} route_type;

/* HTTP route structure */
typedef struct {
    const char *path;
    route_type type;
    const char *(*handler)(const char *request, size_t *response_len);
} http_route;

/* Client state structure */
typedef struct {
    int fd;
    char buffer[BUFFER_SIZE];
    size_t buffer_len;
    const char *response;
    size_t response_len;
    int response_sent;
} client_state;

/* Global variables */
static client_state *clients[MAX_CLIENTS];
static aeEventLoop *el;

/* Function prototypes */
void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask);
void readFromClient(aeEventLoop *el, int fd, void *privdata, int mask);
void writeToClient(aeEventLoop *el, int fd, void *privdata, int mask);
void serverCron(aeEventLoop *el, long long id, void *clientData);
client_state* createClient(int fd);
void freeClient(client_state *client);
int setNonBlock(int fd);
const char *routeRequest(const char *request, route_type *type, size_t *response_len);

/* Light route handlers */
const char *handleHome(const char *request, size_t *response_len);
const char *handleAbout(const char *request, size_t *response_len);

/* Heavy route handlers */
const char *handleHeavyComputation(const char *request, size_t *response_len);
const char *handleLargeFileRead(const char *request, size_t *response_len);

/* Define routes */
http_route routes[] = {
    {"/", ROUTE_LIGHT, handleHome},
    {"/about", ROUTE_LIGHT, handleAbout},
    {"/heavy", ROUTE_HEAVY, handleHeavyComputation},
    {"/largefile", ROUTE_HEAVY, handleLargeFileRead},
    {NULL, ROUTE_LIGHT, NULL} /* End of routes */
};

/* Set a socket to non-blocking mode */
int setNonBlock(int fd) {
    int flags;
    
    if ((flags = fcntl(fd, F_GETFL)) == -1) {
        perror("fcntl(F_GETFL)");
        return -1;
    }
    
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl(F_SETFL)");
        return -1;
    }
    
    return 0;
}

/* Create a new client structure */
client_state* createClient(int fd) {
    client_state* client = malloc(sizeof(client_state));
    if (!client) {
        perror("malloc");
        return NULL;
    }
    
    client->fd = fd;
    client->buffer_len = 0;
    client->response = NULL;
    client->response_len = 0;
    client->response_sent = 0;
    memset(client->buffer, 0, BUFFER_SIZE);
    
    return client;
}

/* Free a client and close its connection */
void freeClient(client_state *client) {
    if (!client) return;
    
    printf("Client disconnected (fd=%d)\n", client->fd);
    
    /* Remove the client from event loop */
    aeDeleteFileEvent(el, client->fd, AE_READABLE);
    aeDeleteFileEvent(el, client->fd, AE_WRITABLE);
    
    /* Close the socket */
    close(client->fd);
    
    /* Free client data */
    free(client);
    clients[client->fd] = NULL;
}

/* Process a request and route it to the appropriate handler */
const char *routeRequest(const char *request, route_type *type, size_t *response_len) {
    char method[16] = {0};
    char path[256] = {0};
    char version[16] = {0};
    
    /* Parse the request line */
    sscanf(request, "%15s %255s %15s", method, path, version);
    
    printf("Received request: %s %s %s\n", method, path, version);
    
    /* Find matching route */
    for (int i = 0; routes[i].path != NULL; i++) {
        if (strcmp(path, routes[i].path) == 0) {
            *type = routes[i].type;
            return routes[i].handler(request, response_len);
        }
    }
    
    /* Default 404 response */
    const char *not_found_response = 
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 9\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Not Found";
    
    *type = ROUTE_LIGHT;
    *response_len = strlen(not_found_response);
    return not_found_response;
}

/* Structure to pass to threadpool for processing requests */
typedef struct {
    aeEventLoop *el;
    client_state *client;
    char *request;
} request_task;

/* Thread function to process heavy requests */
void processRequestInThread(void *arg) {
    request_task *task = (request_task *)arg;
    client_state *client = task->client;
    aeEventLoop *el = task->el;
    char *request = task->request;
    
    /* Process the request (this could be CPU intensive or I/O bound) */
    route_type type;
    size_t response_len;
    const char *response = routeRequest(request, &type, &response_len);
    
    /* Store the response in the client structure */
    client->response = response;
    client->response_len = response_len;
    
    /* Register for write event to send the response */
    aeCreateFileEvent(el, client->fd, AE_WRITABLE, writeToClient, client, TASK_LIGHT);
    
    /* Free task resources */
    free(request);
    free(task);
}

/* Write to client callback */
void writeToClient(aeEventLoop *el, int fd, void *privdata, int mask) {
    client_state *client = (client_state*) privdata;
    
    /* If we have a response, send it */
    if (client->response && client->response_len > 0) {
        ssize_t written = write(fd, 
                               client->response + client->response_sent, 
                               client->response_len - client->response_sent);
        
        if (written <= 0) {
            if (written == -1 && errno == EAGAIN) return; /* Try again later */
            
            /* Error or connection closed */
            freeClient(client);
            return;
        }
        
        client->response_sent += written;
        
        /* If we've sent the entire response, close the connection */
        if (client->response_sent >= client->response_len) {
            freeClient(client);
        }
    } else {
        /* No response available yet, should not happen */
        freeClient(client);
    }
}

/* Handle reading from client */
void readFromClient(aeEventLoop *el, int fd, void *privdata, int mask) {
    client_state *client = (client_state*) privdata;
    char buf[BUFFER_SIZE];
    int nread;
    
    nread = read(fd, buf, BUFFER_SIZE - client->buffer_len - 1);
    if (nread <= 0) {
        if (nread == -1 && errno == EAGAIN) return; /* Try again later */
        
        /* Error or connection closed */
        freeClient(client);
        return;
    }
    
    /* Append to buffer */
    if (client->buffer_len + nread < BUFFER_SIZE) {
        memcpy(client->buffer + client->buffer_len, buf, nread);
        client->buffer_len += nread;
        client->buffer[client->buffer_len] = '\0';
    }
    
    /* Check if we have a complete HTTP request */
    if (strstr(client->buffer, "\r\n\r\n")) {
        /* Remove read event handler until we've sent the response */
        aeDeleteFileEvent(el, fd, AE_READABLE);
        
        /* Determine if this is a heavy or light request */
        route_type type;
        size_t response_len;
        const char *req_copy = strdup(client->buffer);
        const char *response = routeRequest(client->buffer, &type, &response_len);
        
        if (type == ROUTE_HEAVY) {
            printf("Processing heavy request on fd %d via thread pool\n", fd);
            
            /* Create a task for the thread pool */
            request_task *task = malloc(sizeof(request_task));
            if (task != NULL) {
                task->el = el;
                task->client = client;
                task->request = (char*)req_copy;
                
                /* Add the task to the thread pool */
                threadpool_add(el->threadpool, processRequestInThread, task, 1);
            } else {
                /* If task allocation fails, fall back to synchronous processing */
                free((void*)req_copy);
                client->response = response;
                client->response_len = response_len;
                aeCreateFileEvent(el, fd, AE_WRITABLE, writeToClient, client, TASK_LIGHT);
            }
        } else {
            /* Process light request immediately in the main thread */
            free((void*)req_copy);
            client->response = response;
            client->response_len = response_len;
            aeCreateFileEvent(el, fd, AE_WRITABLE, writeToClient, client, TASK_LIGHT);
        }
    }
}

/* Accept a new client connection */
void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd;
    
    /* Accept the connection */
    client_fd = accept(fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            perror("accept");
        return;
    }
    
    /* Set to non-blocking mode */
    if (setNonBlock(client_fd) == -1) {
        close(client_fd);
        return;
    }
    
    /* Check if we can handle more clients */
    if (client_fd >= MAX_CLIENTS) {
        fprintf(stderr, "Too many clients, closing connection\n");
        close(client_fd);
        return;
    }
    
    /* Create client structure */
    client_state *client = createClient(client_fd);
    if (!client) {
        close(client_fd);
        return;
    }
    
    /* Store client */
    clients[client_fd] = client;
    
    /* Register for read events */
    if (aeCreateFileEvent(el, client_fd, AE_READABLE, readFromClient, client, TASK_LIGHT) == AE_ERR) {
        fprintf(stderr, "Error registering event for client\n");
        freeClient(client);
        return;
    }
    
    printf("Accepted connection from %s:%d (fd=%d)\n", 
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), client_fd);
}

/* Initialize a TCP server */
int initServer(void) {
    int server_sock;
    struct sockaddr_in server_addr;
    
    /* Create socket */
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("socket");
        return -1;
    }
    
    /* Set socket options */
    int yes = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        close(server_sock);
        return -1;
    }
    
    /* Set non-blocking */
    if (setNonBlock(server_sock) == -1) {
        close(server_sock);
        return -1;
    }
    
    /* Bind */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);
    
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_sock);
        return -1;
    }
    
    /* Listen */
    if (listen(server_sock, BACKLOG) == -1) {
        perror("listen");
        close(server_sock);
        return -1;
    }
    
    printf("Server listening on port %d\n", SERVER_PORT);
    return server_sock;
}

/* Server initialization before sleep callback */
void beforeSleep(aeEventLoop *el) {
    /* This function will be called before the event loop sleeps */
    /* We could perform periodic tasks here if needed */
}

/* Server cron job - runs every 1000ms */
int serverCron(aeEventLoop *el, long long id, void *clientData) {
    static long long counter = 0;
    
    counter++;
    
    /* Log active connections and thread pool stats every 5 seconds */
    if (counter % 5 == 0) {
        int active_clients = 0;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] != NULL) active_clients++;
        }
        
        int tasks_processed = threadpool_get_tasks_processed(el->threadpool);
        int queue_size = threadpool_get_queue_size(el->threadpool);
        
        printf("=== Server Stats ===\n");
        printf("Active connections: %d\n", active_clients);
        printf("Thread pool tasks processed: %d\n", tasks_processed);
        printf("Thread pool queue size: %d\n", queue_size);
        printf("===================\n");
    }
    
    return 1000; /* Call again in 1000ms */
}

/* Route handlers */
const char *handleHome(const char *request, size_t *response_len) {
    static const char *response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 205\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<html><body><h1>Welcome to the kqueue-based Web Server</h1>"
        "<p>This is a simple HTTP server using kqueue and thread pool.</p>"
        "<p><a href=\"/heavy\">Test heavy computation</a></p>"
        "<p><a href=\"/largefile\">Test large file</a></p>"
        "</body></html>";
    
    *response_len = strlen(response);
    return response;
}

const char *handleAbout(const char *request, size_t *response_len) {
    static const char *response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 148\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<html><body><h1>About</h1>"
        "<p>This server demonstrates event-driven programming with kqueue and "
        "handling heavy tasks with a thread pool.</p>"
        "</body></html>";
    
    *response_len = strlen(response);
    return response;
}

/* Simulate a CPU-intensive task */
const char *handleHeavyComputation(const char *request, size_t *response_len) {
    /* Simulate heavy CPU work by calculating prime numbers */
    int count = 0;
    int i, j, isPrime;
    
    printf("Starting heavy computation...\n");
    
    /* Find the first 10000 prime numbers (CPU intensive) */
    for (i = 2; count < 10000; i++) {
        isPrime = 1;
        for (j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                isPrime = 0;
                break;
            }
        }
        if (isPrime) {
            count++;
        }
    }
    
    printf("Finished heavy computation, found %d primes up to %d\n", count, i);
    
    static char response[1024];
    sprintf(response, 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<html><body><h1>Heavy Computation Complete</h1>"
        "<p>Found %d prime numbers up to %d.</p>"
        "<p>This computation was performed in a worker thread.</p>"
        "<p><a href=\"/\">Back to home</a></p>"
        "</body></html>",
        200 + 20, count, i);
    
    *response_len = strlen(response);
    return response;
}

/* Simulate reading a large file */
const char *handleLargeFileRead(const char *request, size_t *response_len) {
    /* Simulate slow I/O operation by sleeping */
    printf("Starting simulated large file read...\n");
    
    /* Sleep for 2 seconds to simulate disk I/O */
    sleep(2);
    
    printf("Finished simulated large file read\n");
    
    static const char *response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 215\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<html><body><h1>Large File Read Complete</h1>"
        "<p>This simulated a slow I/O operation that would block the event loop.</p>"
        "<p>It was processed in a worker thread to keep the server responsive.</p>"
        "<p><a href=\"/\">Back to home</a></p>"
        "</body></html>";
    
    *response_len = strlen(response);
    return response;
}

int main(int argc, char **argv) {
    int server_sock;
    
    /* Initialize client array */
    memset(clients, 0, sizeof(clients));
    
    /* Create event loop with thread pool */
    el = aeCreateEventLoop(MAX_CLIENTS, THREAD_POOL_SIZE);
    if (el == NULL) {
        fprintf(stderr, "Failed to create event loop\n");
        return 1;
    }
    
    /* Initialize server */
    server_sock = initServer();
    if (server_sock == -1) {
        fprintf(stderr, "Failed to initialize server\n");
        aeDeleteEventLoop(el);
        return 1;
    }
    
    /* Register server socket for read events */
    if (aeCreateFileEvent(el, server_sock, AE_READABLE, acceptTcpHandler, NULL, TASK_LIGHT) == AE_ERR) {
        fprintf(stderr, "Failed to register server socket\n");
        close(server_sock);
        aeDeleteEventLoop(el);
        return 1;
    }
    
    /* Create a periodic time event for server cron */
    aeCreateTimeEvent(el, 1000, serverCron, NULL, NULL, TASK_LIGHT);
    
    /* Set before sleep function */
    aeSetBeforeSleepProc(el, beforeSleep);
    
    printf("Server started successfully with %d worker threads. Event loop running...\n", THREAD_POOL_SIZE);
    
    /* Main event loop */
    aeMain(el);
    
    /* Cleanup */
    close(server_sock);
    aeDeleteEventLoop(el);
    
    return 0;
}
