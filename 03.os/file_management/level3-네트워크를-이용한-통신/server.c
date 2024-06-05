#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>  // Include this header for 'write' and 'close'

int main()
{
    int sp, sa;

    struct sockaddr_in ad;

    memset(&ad, 0, sizeof(ad));
    ad.sin_family = AF_INET;
    ad.sin_addr.s_addr = htonl(INADDR_ANY);
    ad.sin_port = htons(11234);

    sp = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);  // Change SOCKET_STREAM to SOCK_STREAM
    bind(sp, (struct sockaddr *) &ad, sizeof(ad));

    while (1) {
        listen(sp, 10);
        sa = accept(sp, 0, 0);
        write(sa, "Test", 4);  // Write 4 characters (without null terminator)
        close(sa);
    }

    return 0;
}

