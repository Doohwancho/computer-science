#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include "clientObject.h"
#include "listen.h"
#include "socketsNetLib.h"
#include "send.h"

#define SEND_LOCK_COUNT 32


// Global switch for how packets should be sent
PacketSenderType packetSenderType = PACKET_SENDER_TCP;

typedef ssize_t (*PacketSender)(PacketSendingArgs*);
static inline ssize_t send_TCP_unencrypted (PacketSendingArgs *args);
static inline ssize_t send_TCP_encrypted   (PacketSendingArgs *args);
static int            waitForReady         (int sockfd);
static PacketSender send_variants[PACKET_SENDER_COUNT] = 
{ 
            send_TCP_unencrypted,
            send_TCP_encrypted
};


// We use these locks to make each TCP transfer on a socket a critical section
// and do it on multiple sockets simultaneously.
static pthread_mutex_t sendLocks[SEND_LOCK_COUNT]  = {PTHREAD_MUTEX_INITIALIZER};

void setTCP_sendType (PacketSenderType type)
{
    packetSenderType = type;
}

int sendDataUDP(const char *data, const ssize_t datasize, Host *remotehost)
{
#ifdef DEBUG
    fprintf(stderr, "\nSending UDP packet...");
#endif
    socketfd  sockfd                     = getSocket(remotehost);
    socklen_t len                        = sizeof(remotehost->address);    
    const struct sockaddr* remoteAddress = NULL;

    remoteAddress = (const struct sockaddr *) &remotehost->address; 

    // sendto (sockfd, data, datasize, MSG_CONFIRM, remoteAddress, len);
    sendto (sockfd, data, datasize, 0, remoteAddress, len); //TODO - MSG_CONFIRM이 linux에서만 되고 mac에선 안되서 0으로 넣음. 
    
    return SUCCESS;
}

static inline ssize_t send_TCP_unencrypted (PacketSendingArgs *args)
{
    return send(getSocket(args->remotehost), args->buffer, args->bytesToProcess, 0);
}
static inline ssize_t send_TCP_encrypted (PacketSendingArgs *args)
{
    return SSL_write(getHostSSL(args->remotehost), args->buffer, args->bytesToProcess);
}
static int waitForReady(int sockfd)
{
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(sockfd, &writefds);

    int selectStatus = select(sockfd + 1,
                              NULL,
                              &writefds,
                              NULL,
                              NULL);
    if (selectStatus < 0) {
        perror("Select error");
        return -1;
    }
    return selectStatus > 0;
}

//TODO - takeaway: how sendDataTCP() works?
//data를 remotehost에 TCP protocol로 보내는 함수
//lock을 쓴다. 여러 쓰레드가 동시에 한 객체에 접근하나보다. 
//while loop 안에서 datasize일 때까지 data를 byte로 변환해서 보내는 것 같긴 한데, 어떻게 보냄?
//send_variants[]가 함수를 2분기 쳐서 send_TCP_unencrypted로 보내는 것인 듯?
//send_variants() 함수에 파라미터로 이걸 넣음. {remotehost, data, datasize}
//그러면, send_TCP_unencrypted()에서 send(getSocket()); 함 
//getSocket(remotehost)는 remosthost에 달린 소켓 불러오는 거고, 그 타겟 소켓으로 TCP packet을 send()함 
//send()는 stdlib에 구현된거라 직접 구현하는건 아닌 듯. 
//다만, send()로 보냈을 때, 그 결과 status가 -1, 0, 양수인지에 따라 에러 핸들링 하는건 개발자의 몫인 듯 
//status = total bytes sent. 이게 결국 datasize와 같아지면 sendDataTCP()가 끝남. 
int sendDataTCP(const char *data, const size_t datasize, Host *remotehost)
{
#ifdef DEBUG
    fprintf(stderr, "\nSending TCP packet...");
#endif
    int               lockIndex      = getHostID(remotehost) % SEND_LOCK_COUNT;
    ssize_t           totalBytesSent = 0;
    ssize_t           status         = 0;
    PacketSendingArgs sendArgs       = {remotehost, data, datasize};

    pthread_mutex_lock(&sendLocks[lockIndex]); //lock index를 여러 쓰레드가 값을 업데이트 하기 때문에, 락걸어줌
    while (totalBytesSent < datasize) {
        // A switch to send packets in different ways
        status = send_variants[packetSenderType](&sendArgs);

        if (status == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                if (waitForReady(getSocket(remotehost)) < 0) {
                    perror              ("\nSelect error");
                    closeConnections    (remotehost);
                    pthread_mutex_unlock(&sendLocks[lockIndex]);
                    return ERROR;
                }
                continue;
            }

            perror               ("\nFailed to send TCP message");
            closeConnections     (remotehost);
            pthread_mutex_unlock (&sendLocks[lockIndex]);
            return ERROR;
        }
        if (status == 0) {
            fprintf          (stderr, "\nSocket is closed, couldn't send data");
            closeConnections (remotehost);
            pthread_mutex_unlock(&sendLocks[lockIndex]);
            return SUCCESS;
        }
        totalBytesSent += status;
    }
    pthread_mutex_unlock(&sendLocks[lockIndex]);
    return SUCCESS;
}

/*
 * TODO: Not currently working,
 * make it work.
 */
int sendDataTCPandRecv(const char *data, 
                       const ssize_t datasize, 
                       Host *remotehost, 
                       void (*packet_handler)(char*, ssize_t, Host*))
{
    packetReceptionArgs *recvArgs  = NULL;

    sendDataTCP(data, datasize, remotehost);

    recvArgs = (packetReceptionArgs*)calloc(1, sizeof(packetReceptionArgs));
    if (recvArgs == NULL) {
        closeConnections(remotehost);
        return SEND_ERROR;
    }

    // Remotehost pointer is now owned by new thread
    recvArgs->remotehost     = remotehost;
    recvArgs->localhost      = NULL;
    recvArgs->packet_handler = packet_handler;
    recvArgs->bytesToProcess = 0;
    recvArgs->buffer         = (char*)calloc(PACKET_BUFFER_SIZE*2, sizeof(char));
    if (recvArgs->buffer == NULL) {
        free (recvArgs);
        return SEND_ERROR;
    }
        
    receiveTCPpackets (recvArgs);
    return SUCCESS;
}

/*
 * Returns the amount of bytes
 * sent, or -1 for an error OR -2
 * Call this function until it errors
 * or sends datasize bytes
 */
static ssize_t sendDataTCP_NB(const char *data, 
                              const ssize_t datasize, 
                              Host *remotehost)
{
#ifdef DEBUG
    fprintf(stderr, "\nSending TCP packet...");
#endif
    int               lockIndex      = getHostID(remotehost) % SEND_LOCK_COUNT;
    ssize_t           status         = 0;
    PacketSendingArgs sendArgs       = {remotehost, data, datasize};
    pthread_mutex_lock   (&sendLocks[lockIndex]);
        // A switch to send packets in different ways
    status = send_variants[packetSenderType](&sendArgs);

    if (status == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return SEND_TRYAGAIN;
        }
        perror("\nFailed to send TCP message");
        closeConnections     (remotehost);
        pthread_mutex_unlock (&sendLocks[lockIndex]);
        return SEND_ERROR;
    }
    if (status == 0) {
        fprintf          (stderr, "\nSocket is closed, couldn't send data");
        closeConnections (remotehost);
        pthread_mutex_unlock   (&sendLocks[lockIndex]);
        return SUCCESS;
    }
    pthread_mutex_unlock   (&sendLocks[lockIndex]);
    return status;
}

int multicastTCP(const char *data, const ssize_t datasize, int cacheIndex)
{
#ifdef DEBUG
    fprintf(stderr, "\nMulticasting to cache number %d...", cacheIndex);
#endif
    Host      *remotehost  = NULL;
    bool       tryAgain    = 1;
    ssize_t    currentStatus = 0;
    ssize_t    statuses[MAX_HOSTS_PER_CACHE] = { 0 };

    for (int i = 0; i < MAX_HOSTS_PER_CACHE; i++) {
        statuses[i] = SEND_TRYAGAIN;
    }
    while (tryAgain == true) {
        tryAgain = false;
        for (int i = 0; i < MAX_HOSTS_PER_CACHE; i++) {
            remotehost    = getHostFromCache(cacheIndex, i);
            if (remotehost == NULL) {
                continue;
            }
            currentStatus = statuses[i];
            if ((currentStatus < datasize && currentStatus > 0)
                || currentStatus == SEND_TRYAGAIN) {
                currentStatus =
                sendDataTCP_NB(data, datasize, remotehost);
                tryAgain = (currentStatus < datasize 
                           && currentStatus > 0
                           && tryAgain == 0)
                           || currentStatus == SEND_TRYAGAIN;
            }
        }
    }
    return SUCCESS;
}

int connectToTCP(Host *remotehost)
{
    const socketfd   sockfd        = createSocket(SOCK_DEFAULT_TCP);
    struct sockaddr* remoteAddress = (struct sockaddr *)&remotehost->address;
    const socklen_t  sizeOfAddress = sizeof(remotehost->address);
    if (FAILURE(connect(sockfd, remoteAddress, sizeOfAddress))) {
        perror("\nConnect failed");
        return ERROR;
    }
    setSocket        (remotehost, sockfd);
    return SUCCESS;
}
