#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

int main()
{
    int sp;
    char buf[5];
    struct sockaddr_in ad;

    sp = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    memset(&ad, 0, sizeof(ad));
    ad.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &(ad.sin_addr));  // Use inet_pton instead of inet_addr
    ad.sin_port = htons(11234);

    connect(sp, (struct sockaddr *)&ad, sizeof(ad));  // Use &ad instead of *ad

    read(sp, buf, 4);  // Read 4 characters
    buf[4] = '\0';  // Add null terminator to the buffer
    std::cout << buf;

    close(sp);
    exit(0);
}

