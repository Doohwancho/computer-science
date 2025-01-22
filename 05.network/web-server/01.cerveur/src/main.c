#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "HTTP_Server.h"
#include "Routes.h"
#include "Response.h"

int main() {
	// initiate HTTP_Server
	HTTP_Server http_server;
	init_server(&http_server, 6969);

	int client_socket;

	// registering Routes
	struct Route * route = initRoute("/", "index.html");
	addRoute(route, "/about", "about.html");


	printf("\n====================================\n");
	printf("=========ALL VAILABLE ROUTES========\n");
	// display all available routes
	inorder(route);

	while (1) {
		char client_msg[4096] = ""; //4kb buffer

		client_socket = accept(http_server.socket, NULL, NULL);

		read(client_socket, client_msg, 4095); //read client http request
		printf("%s\n", client_msg);

		// parsing client socket header to get HTTP method, route
		char *method = "";
		char *urlRoute = "";

		char *client_http_header = strtok(client_msg, "\n"); //split client_msg by new line. 아 이래서 GET / HTTP/1.1 밑에 한칸 띄워져 있는거구나

		printf("\n\n%s\n\n", client_http_header);

		char *header_token = strtok(client_http_header, " "); //header를 한번 더 파싱

		int header_parse_counter = 0;

		while (header_token != NULL) {

			switch (header_parse_counter) {
				case 0:
					method = header_token;
				case 1:
					urlRoute = header_token;
			}
			header_token = strtok(NULL, " ");
			header_parse_counter++;
		}

		printf("The method is %s\n", method);
		printf("The route is %s\n", urlRoute);


		char template[100] = "";

		if (strstr(urlRoute, "/static/") != NULL) {
			//strcat(template, urlRoute+1);
			strcat(template, "static/index.css"); //이건 하드코딩임. 나중에 수정해야함.
		}else {
			struct Route * destination = search(route, urlRoute); //search for the route in the tree
			strcat(template, "templates/");

			if (destination == NULL) {
				strcat(template, "404.html"); //if route not found, return 404
			}else {
				strcat(template, destination->value);
			}
		}

		char * response_data = render_static_file(template); //render the template

		char http_header[4096] = "HTTP/1.1 200 OK\r\n\r\n"; //http header

		strcat(http_header, response_data); //concatenate http header and response data
		strcat(http_header, "\r\n\r\n");


		send(client_socket, http_header, sizeof(http_header), 0); //send response to client
		close(client_socket); //close client socket
		free(response_data); //free response data
	}
	return 0;
}
