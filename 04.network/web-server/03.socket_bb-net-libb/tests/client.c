#include <stdio.h>
#include <string.h>
#include "bbnetlib.h"
#include <unistd.h>
#include <time.h>

void sleep_ms(unsigned int milliseconds) {
    struct timespec req;
    req.tv_sec = milliseconds / 1000;
    req.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&req, NULL);
}

int main(void)
{
    printf("\nWelcome to the chat client!");
    printf("\n-------------------------------\n");
    Host remotehost = createHost("127.0.0.1", 80);
    connectToTCP(remotehost);

    char message[1024];
    while (1) {
        printf("Enter message (or 'quit' to exit): ");
        fgets(message, sizeof(message), stdin);
        
        // Remove newline character
        size_t len = strlen(message);
        if (len > 0 && message[len-1] == '\n') {
            message[len-1] = '\0';
        }
        
        if (strcmp(message, "quit") == 0) {
            break;
        }
        
        sendDataTCP(message, strlen(message), remotehost);
        
        // Here you would typically add code to receive and display messages from the server
        // For example:
        // char received[1024];
        // ssize_t bytes_received = receiveDataTCP(received, sizeof(received), remotehost);
        // if (bytes_received > 0) {
        //     received[bytes_received] = '\0';
        //     printf("Received: %s\n", received);
        // }
    }

    sendDataTCP("Disconnecting", 13, remotehost);
    // Close connection here

    return 0;
}

