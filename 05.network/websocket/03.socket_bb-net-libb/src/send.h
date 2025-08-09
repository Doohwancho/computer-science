#ifndef NETLIB_SEND
#define NETLIB_SEND

#include "clientObject.h"

#define SEND_TRYAGAIN -2
#define SEND_ERROR    -1

typedef struct {
    Host       *remotehost;
    const char *buffer;
    ssize_t     bytesToProcess;
} PacketSendingArgs;

typedef enum {
    PACKET_SENDER_TCP, //0
    PACKET_SENDER_TLS, //1
    PACKET_SENDER_COUNT //2
}PacketSenderType;

BBNETAPI int sendDataUDP  (const char *data, 
                           const ssize_t datasize, 
                           Host *remotehost);
BBNETAPI int sendDataTCP  (const char *data, 
                           const size_t datasize, 
                           Host *remotehost);
BBNETAPI int connectToTCP (Host *remotehost);


void setTCP_sendType (PacketSenderType type);

#endif
