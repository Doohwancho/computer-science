#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
// #include <malloc.h> 
#include <stdlib.h> //TODO - malloc.c is not supported on mac, therefore change it to #include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

#include "clientObject.h"
#include "listen.h"
#include "defines.h"
#include "encryption.h"
#include "socketsNetLib.h"
#include "threadPool.h"
#include "send.h"

threadPool TCPthreadPool = NULL;
threadPool UDPthreadPool = NULL;

// global sslContext TCP... move this?
static SSL_CTX *sslContext = NULL;

static void sigpipeIgnorer();

// Global switch for what type of TCP recv() we'll be doing
static PacketReceiverType packetReceiverType = PACKET_RECEIVER_TCP;

typedef ssize_t (*PacketReceiver)(packetReceptionArgs*);
static inline ssize_t recv_TCP_unencrypted (packetReceptionArgs *restrict args);
static inline ssize_t recv_TCP_encrypted   (packetReceptionArgs *restrict args);
static PacketReceiver recv_variants[PACKET_RECEIVER_COUNT] = 
{ 
            recv_TCP_unencrypted,
            recv_TCP_encrypted
};

static void destroyPacketReceptionArgs (packetReceptionArgs **args);

void setTCP_receiveType(PacketReceiverType type)
{
    packetReceiverType = type;
}

int listenForUDP(Host *localhost, void (*packet_handler)(char*, ssize_t, Host*))    //Should be called on it's own thread as the while loop is blocking 
{    
    const socketfd sockfd                     = createSocket(SOCK_DEFAULT_UDP);
    Host           remotehost                 = { 0 };
    ssize_t        numBytes                   = 0;
    socklen_t      len                        = 0;
    char           buffer[PACKET_BUFFER_SIZE] = { 0 };

    if (FAILURE(bindSocket(sockfd, 
                           localhost)))
    {
        perror("Failed to listen for UDP \n");
        return ERROR;
    }

    setCommunicating(localhost);
    setCommunicating(&remotehost);

    len = sizeof(localhost->address);
    // Need to cast the pointer to a sockaddr type to satisfy the syscall
    struct sockaddr* remoteAddress = ( struct sockaddr *)&remotehost.address;

    while(isCommunicating(localhost)) {
        numBytes = recvfrom(sockfd, 
                            buffer, 
                            PACKET_BUFFER_SIZE, 
                            MSG_WAITALL, 
                            remoteAddress, 
                           &len);

        if (numBytes < PACKET_BUFFER_SIZE) {
            buffer[numBytes] = '\0';    // Null terminate the received data
        }

        if (numBytes == 0) {
            printf("Transmission finished\n");
            break;
        }
        // TODO: this is currently blocking and should run in it's own thread from
        // the pool, but I don't need UDP right now and even if I did it should be _okay_
        // for a small amount of hosts.
        packet_handler(buffer, numBytes, &remotehost);
    }

    if (isCommunicating(localhost)) {
        closeConnections(localhost);
    }
    if (isCommunicating(&remotehost)) {
        closeConnections(&remotehost);
    }

    return SUCCESS;
}
static int tryAcceptConnection(Host *localhost, Host *remotehost)
{
    socklen_t        addrLen       = 0;
    struct sockaddr *remoteAddress = NULL;
    socketfd         sockfd        = 0;
    int              err           = 0;
    // Need to cast the type because that's
    // just how sockets work
    remoteAddress = ( struct sockaddr *)&remotehost->address;
    addrLen       = sizeof(remotehost->address);

    sockfd = 
    createSocket(accept(getSocket(localhost), 
                 remoteAddress, 
                 &addrLen));
    if (sockfd < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return RECV_TRYAGAIN;
        }
        return RECV_ERROR;
    }
    err =
    setSocketNonBlock(sockfd);
    if (err != 0) {
        return RECV_ERROR;
    }
    setSocket        (remotehost, sockfd);
    setCommunicating (remotehost);
    return 0;
}

static void sigpipeIgnorer()
{
    static sigset_t set;
    sigemptyset (&set);
    sigaddset(&set, SIGPIPE);
    if (pthread_sigmask(SIG_BLOCK, &set, NULL) != 0) {
        fprintf(stderr, "\nError setting SIGPIPE signal mask. Exiting...\n");
        exit(1);
    }
}
/*
 * TODO: takeaway: listenForTCP() 함수가 어떻게 작동하는피 단계별로 주석에 적어놓음 

 * Core TCP listening function.
 * Call it once and it will block
 * and listen until the localhost
 * is set not to listen any more.
 * */
int listenForTCP(Host *localhost, 
                 void (*packet_handler)(char*, ssize_t, Host*))
{
#ifdef DEBUG
    fprintf(stderr, "\nListening for TCP connections...");
#endif
    packetReceptionArgs *receptionArgs = NULL;
    Host                *remotehost    = NULL;
    int                  er            = 0;

    sigpipeIgnorer(); //It first calls sigpipeIgnorer() to block SIGPIPE signals, which are raised when trying to write to a socket that has been closed on the other end

    createThreadPool  (&TCPthreadPool); // A thread pool 1 is created to manage multiple TCP connections concurrently.
    setSocket         (localhost, createSocket(SOCK_DEFAULT_TCP)); //A TCP socket is created 
    setSocketNonBlock (getSocket(localhost)); //and set to non-blocking mode using createSocket and setSocketNonBlock.

    /* TLS setup */ 
    if (packetReceiverType == PACKET_RECEIVER_TLS) {
        sslContext = createSSLContext();
        configureSSLContext(sslContext);
    }
    /* --------- */

    if (FAILURE(bindSocket(getSocket(localhost), localhost))) { //localhost와 소켓을 bind 
        perror("\nFailed to listen for TCP");
        return ERROR;
    }
    setCommunicating(localhost);
    if (FAILURE(listen(getSocket(localhost), SOCK_BACKLOG))) { //해당 소켓을 listen 시작 
        printf("\nError: listen() failed with errno %d: %s\n", errno, strerror(errno));
        return ERROR;
    }

    while(isCommunicating(localhost)) 
    {
        remotehost = createHost("", 0000);
        if (remotehost == NULL) {
            fprintf(stderr, "\nFatal error.\n");
            goto early_exit;
        }
        er = RECV_TRYAGAIN;
        while (er == RECV_TRYAGAIN) {
            er =
            tryAcceptConnection (localhost, remotehost);
        }
        if (er == RECV_ERROR) {
            destroyHost(&remotehost);
            continue;
        }
        if (packetReceiverType == PACKET_RECEIVER_TLS) {
            if(FAILURE(attemptTLSHandshake (remotehost, sslContext))) {; //if accepted connection, do TLS handshakes
                perror("\nError: TLS Handshake failed.\n");
                destroyHost(&remotehost);
                continue;
            }
        }

        receptionArgs = (packetReceptionArgs*)calloc(1, sizeof(packetReceptionArgs)); //client에서 보내는 것을 'packetReceptionArgs' 구조체로 받아서 
        if (receptionArgs == NULL) {
            goto early_exit;
        }
        // Remotehost pointer is now owned by new thread
        receptionArgs->remotehost     = remotehost;
        receptionArgs->localhost      = localhost;
        receptionArgs->packet_handler = packet_handler;
        receptionArgs->bytesToProcess = 0;
        receptionArgs->buffer         = (char*)calloc(PACKET_BUFFER_SIZE, sizeof(char));
        if (receptionArgs->buffer == NULL) {
            destroyHost (&remotehost);
            free        (receptionArgs);
            goto early_exit;
        }

        addTaskToThreadPool(TCPthreadPool, (void*)receiveTCPpackets, receptionArgs); //만들어 둔 thread pool에 1000 size task_queue에 넣는다. 물론 이 때 mutex lock을 쓴다. 
        //1. create new thread to handle listening to that specific connection. 
        //2. This allows the server to handle multiple connections concurrently by delegating the task of receiving data from each connection to separate threads.

    }

//if error occurs and server stops communicating, it cleans up by 
//1. destroying the thread pool
//2. close socket
//3. free SSL context
//4. destroy localhost object 
early_exit: 
    destroyThreadPool (&TCPthreadPool);
    closeSocket       (getSocket(localhost));
    SSL_CTX_free      (sslContext);
    destroyHost       (&localhost);
    return SUCCESS;
}
static inline ssize_t recv_TCP_unencrypted(packetReceptionArgs *restrict args)
{
    ssize_t numBytes = 0;
    numBytes = recv(getSocket(args->remotehost), 
                     args->buffer, 
                     PACKET_BUFFER_SIZE, 
                     0);
    return numBytes;
}
static inline ssize_t recv_TCP_encrypted(packetReceptionArgs *restrict args)
{
    ssize_t numBytes = 0;
    numBytes = SSL_read(getHostSSL(args->remotehost), 
                        args->buffer, 
                        PACKET_BUFFER_SIZE);
    return numBytes;
}
void receiveTCPpackets(packetReceptionArgs *args) 
{
#ifdef DEBUG
    fprintf(stderr, "\nReceiving a TCP packet in a thread...");
#endif
    ssize_t numBytes = 0;
    
    while(isCommunicating(args->remotehost)) {
        // This is supposed to block until bytes are received
        numBytes = recv_variants[packetReceiverType](args);
        if (numBytes >= 0) {
            args->packet_handler(args->buffer, numBytes, args->remotehost); 
            if (numBytes == 0) {
                fprintf(stderr, "\nConnection shutdown triggered by recv()...");
                closeConnections(args->remotehost);
            }
        }
        else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                continue;
            }
            fprintf(stderr, "\nSocket error, closing connection...\n");
            closeConnections(args->remotehost);
        }
    }
    // Let the remote host know we're closing connection
    sendDataTCP                (NULL, 0, args->remotehost);
    closeSocket                (getSocket(args->remotehost));
    destroyPacketReceptionArgs (&args);
    return;
}

/*
 * Precondition that args pointer and members not NULL
 */
static void destroyPacketReceptionArgs(packetReceptionArgs **args)
{
    if (*args == NULL) {
        return;
    }
    destroyHost (&(*args)->remotehost); 
    free        ((*args)->buffer);
    free        (*args);
    *args = NULL;
}
