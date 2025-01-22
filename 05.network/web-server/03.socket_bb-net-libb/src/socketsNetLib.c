#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>

#include "defines.h"
#include "clientObject.h"
#include "socketsNetLib.h"

socketfd createSocket(socketfd value)             
{
    socketfd sockfd = value; //socketfd는 int일 뿐인데, socket file descriptor라고 구분지어 주기 위해 socketfd라는 커스텀 type을 만들어서 부여한 것.
    if (IS_INVALID_FD(socket)) { 
        perror("\nSocket creation failed"); 
        return ERROR;
    }   
    return sockfd;
}

void closeSocket(socketfd sockfd)
{
    close(sockfd);
}

//Q. what is 'binding socket' means?
//A. bind a socket with specific host with port
int bindSocket(socketfd sockfd, Host* localhost)
{
    struct sockaddr* boundAddress = ( struct sockaddr *)&localhost->address;
    if(FAILURE(bind(sockfd, 
                    boundAddress, 
                    sizeof(localhost->address)))) {
        perror("\nFailed to bind socket!");
        return ERROR;
    }
    return SUCCESS;
}
int setSocketNonBlock(socketfd sockfd)
{    
    int flags = fcntl(sockfd, F_GETFL, 0); //fcntl = file control() to get current flag of socket 
    if (flags == -1) {
        return ERROR;
    }
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) { //set socket to non-blocking mode 
        perror("Error setting socket to non-blocking mode");
        return ERROR;
    }
    return SUCCESS; 
}
/*
 * This only works for blocking sockets,
 * don't use.
 */
int setSocketTimeout(socketfd sockfd, long secs)
{
    struct timeval timeout = {};
    timeout.tv_sec  = secs;   // 5 seconds
    timeout.tv_usec = 0;  // 0 microseconds
    if (FAILURE(setsockopt(sockfd, 
                           SOL_SOCKET, 
                           SO_RCVTIMEO, 
                           (char *)&timeout, sizeof(timeout)))) 
    {
        perror("\nSetsockopt error");
    }
    if (FAILURE(setsockopt(sockfd, 
                           SOL_SOCKET, 
                           SO_SNDTIMEO, 
                           (char *)&timeout, sizeof(timeout)))) 
    {
        perror("\nSetsockopt error");
    }

    return 0;
}
