#include "HTTP_Server.h"
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>

void init_server(HTTP_Server * http_server, int port) {
	http_server->port = port;

	int server_socket = socket(AF_INET, SOCK_STREAM, 0); //0 for default protocol, which is TCP

    //what is AF_INET?
    //AF = address family,
    //INET = internet

    //what is SOCK_STREAM?
    //there are 2 types of sockets, stream and datagram
    //datagram is like a single chunk of data in a packet, stream is like a pipe
    //stream is more reliable, datagram is faster
    //for this example, we will use stream

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
    //what is htons?
    //htons = host to network short
    //this is used to convert the port number to a format that the network can understand
    //some host uses big endian, some uses little endian
    //htons allows two different network to communicate, even if they have different byte order.
	server_address.sin_addr.s_addr = INADDR_ANY;

	bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));

	listen(server_socket, 5); //listen for 5 connections

	http_server->socket = server_socket;
	printf("HTTP Server Initialized\nPort: %d\n", http_server->port);
}
