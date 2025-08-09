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

/* Include our event loop library */
/* In a real project this would be in a separate file */
/* The code below assumes the event loop code is in "ae.h" */
#include "my_kqueue.h"

#define SERVER_PORT 8080
#define BACKLOG 128
#define MAX_CLIENTS 1024
#define BUFFER_SIZE 4096

/* HTTP response template */
static const char* http_response = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 12\r\n"
    "Connection: close\r\n"
    "\r\n"
    "Hello World!";

/* Client state structure */
typedef struct {
    int fd;
    char buffer[BUFFER_SIZE];
    size_t buffer_len;
} client_state;

/* Global variables */
static client_state *clients[MAX_CLIENTS];
static aeEventLoop *el;

/* Function prototypes */
void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask);
void readFromClient(aeEventLoop *el, int fd, void *privdata, int mask);
client_state* createClient(int fd);
void freeClient(client_state *client);
int setNonBlock(int fd);

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
    memset(client->buffer, 0, BUFFER_SIZE);
    
    return client;
}

/* Free a client and close its connection */
void freeClient(client_state *client) {
    if (!client) return;
    
    printf("Client disconnected (fd=%d)\n", client->fd);
    
    /* Remove the client from event loop */
    aeDeleteFileEvent(el, client->fd, AE_READABLE);
    
    /* Close the socket */
    close(client->fd);
    
    /* Free client data */
    free(client);
    clients[client->fd] = NULL;
}

/* Handle reading from client */
void readFromClient(aeEventLoop *el, int fd, void *privdata, int mask) {
    client_state *client = (client_state*) privdata;
    char buf[BUFFER_SIZE];
    int nread;
    
    nread = read(fd, buf, BUFFER_SIZE);
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
        printf("Received HTTP request:\n%.*s\n", (int)client->buffer_len, client->buffer);
        
        /* Send HTTP response */
        write(client->fd, http_response, strlen(http_response));
        
        /* Close connection */
        freeClient(client);
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
    if (aeCreateFileEvent(el, client_fd, AE_READABLE, readFromClient, client) == AE_ERR) {
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
void serverCron(aeEventLoop *el) {
    static long long counter = 0;
    
    /* This function will be called periodically, we can perform
     * periodic tasks here, like checking for timeouts, etc. */
    counter++;
    
    if (counter % 10 == 0) {
        /* Log active connections every 10 cycles */
        int active_clients = 0;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] != NULL) active_clients++;
        }
        printf("Active connections: %d\n", active_clients);
    }
}

int main(int argc, char **argv) {
    int server_sock;
    
    /* Initialize client array */
    memset(clients, 0, sizeof(clients));
    
    /* Create event loop */
    el = aeCreateEventLoop(MAX_CLIENTS);
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
    if (aeCreateFileEvent(el, server_sock, AE_READABLE, acceptTcpHandler, NULL) == AE_ERR) {
        fprintf(stderr, "Failed to register server socket\n");
        close(server_sock);
        aeDeleteEventLoop(el);
        return 1;
    }
    
    /* Create a periodic time event to run server tasks */
    aeCreateTimeEvent(el, 1000, /* Run every 1000ms */
        /* Time proc */
        [](aeEventLoop *el, long long id, void *clientData) -> int {
            serverCron(el);
            return 1000; /* Return ms till next call */
        },
        /* Client data */
        NULL);
    
    /* Set before sleep callback */
    aeSetBeforeSleepProc(el, [](aeEventLoop *el) {
        /* This will be called before the event loop sleeps */
    });
    
    printf("Server started successfully. Event loop running...\n");
    
    /* Main event loop */
    aeMain(el);
    
    /* Cleanup */
    close(server_sock);
    aeDeleteEventLoop(el);
    
    return 0;
}
