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
#define BUFFER_SIZE 1024
#define BACKLOG 10  // Maximum length of pending connections queue

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};
    
    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0); //socket.h에 #define 되있는걸 가져와 씀 
    if (server_fd == -1) {
        error("Failed to create socket");
    }
    printf("Socket created successfully\n");
    
    // Set socket options (Optional but recommended)
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        error("setsockopt failed");
    }
    
    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP
    server_addr.sin_port = htons(PORT);
    
    // Bind socket to address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        error("Bind failed");
    }
    printf("Bind successful\n");
    
    // Listen for incoming connections
    if (listen(server_fd, BACKLOG) == -1) {
        error("Listen failed");
    }
    printf("Server listening on port %d\n", PORT);
    
    // Main server loop
    while (1) {
        printf("Waiting for connection...\n");
        
        // Accept incoming connection
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd == -1) {
            perror("Accept failed");
            continue;  // Continue listening for other connections
        }
        
        // Print client information
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
        printf("Connection accepted from %s:%d\n", client_ip, ntohs(client_addr.sin_port));
        
        // Read client message
        ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';  // Null terminate the string
            printf("Received message: %s\n", buffer);
            
            // Send acknowledgment back to client
            const char *response = "Message received\n";
            write(client_fd, response, strlen(response));
        }
        
        // Close client connection
        close(client_fd);
    }
    
    // Close server socket (This part is never reached in this example)
    close(server_fd); //Q. 근데 int type인데 close()하네? free(int 10); 이게 가능한가?  server_fd = socket(AF_INET, SOCK_STREAM, 0); 하는데, 다른 타입으로 형변환 해줘야 하는거 아냐?
                      //A. 이건 free(int 10);은 불가능한데, close()는 소켓 lib에서 소켓 닫으려고 있는 함수라 가능 
    return 0;
}
