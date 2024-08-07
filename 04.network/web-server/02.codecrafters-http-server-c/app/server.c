#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <zlib.h> 

#define BUFFER_SIZE 4096


struct HttpRequest {
    char method[10];
    char path[100];
    char protocol[20];
	char user_agent[BUFFER_SIZE];
	char accepted_encoding[BUFFER_SIZE];
	char body[BUFFER_SIZE];
};

struct HttpResponse {
	char status[BUFFER_SIZE];
	char content_encoding[BUFFER_SIZE];
	char body[BUFFER_SIZE];
};

char *resp_200 = "HTTP/1.1 200 OK\r\n";
char *resp_201 = "HTTP/1.1 201 Created\r\n\r\n";
char *resp_404 = "HTTP/1.1 404 Not Found\r\n\r\n";

char *server_accepted_encoding = "gzip";

void initHttpRequest(struct HttpRequest *request, char *req_buffer);
void initHttpResponse(struct HttpResponse *response, char *status, char *content_encoding, char *body);
void initHeader(char *header_value, size_t header_value_size, char *req_buffer, const char *header_name); 
void processResponse(struct HttpRequest *request, int client, char *directory);
int serverEcho(struct HttpRequest *request, int client);
int requestUserAgent(struct HttpRequest *request, int client);
int requestFile(struct HttpRequest *request, int client, char *directory);
int postFile(struct HttpRequest *request, int client, char *directory);
char *readFile(char *filename);
void writeFile(char *filename, char *content);
char *writeResponse(char *type, struct HttpResponse *response, size_t body_len) ;
char *getDirectoryPath(int argc, char **argv);
int checkIfExistsInArray(char *origin_a[], int origin_a_len, char *value);
char** tokenizer(const char* str, const char* delim, int* count);
void trimString(char *str);
void freeTokens(char** tokens);
int gzip_compress(const char *input, size_t input_size, unsigned char **output, size_t *output_size);
void print_bytes(char *by, size_t len);
char* stringToHex(const char* input);

int uncompressFromGzip(const char *input, int inputSize, char *output, int outputSize) {
    z_stream zs = {0};
    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;
    zs.avail_in = (uInt)inputSize;
    zs.next_in = (Bytef *)input;
    zs.avail_out = (uInt)outputSize;
    zs.next_out = (Bytef *)output;

    inflateInit2(&zs, 16 + MAX_WBITS); // Add 16 to windowBits to enable gzip decoding
    inflate(&zs, Z_FINISH);
    inflateEnd(&zs);

    return zs.total_out;
}


int main(int argc, char **argv) {
	// Disable output buffering
	setbuf(stdout, NULL);

	// You can use print statements as follows for debugging, they'll be visible when running tests.
	printf("Logs from your program will appear here!\n");

	// Uncomment this block to pass the first stage
	//
	int server_fd, client_addr_len;
	struct sockaddr_in client_addr;
	pid_t childpid;
	
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		printf("Socket creation failed: %s...\n", strerror(errno));
		return 1;
	}
	
	// Since the tester restarts your program quite often, setting REUSE_PORT
	// ensures that we don't run into 'Address already in use' errors
	int reuse = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) {
		printf("SO_REUSEPORT failed: %s \n", strerror(errno));
		return 1;
	}
	
	struct sockaddr_in serv_addr = { .sin_family = AF_INET ,
									 .sin_port = htons(4221),
									 .sin_addr = { htonl(INADDR_ANY) },
									};
	
	if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
		printf("Bind failed: %s \n", strerror(errno));
		return 1;
	}
	
	int connection_backlog = 5;
	if (listen(server_fd, connection_backlog) != 0) {
		printf("Listen failed: %s \n", strerror(errno));
		return 1;
	}
	
	printf("Waiting for a client to connect...\n");

	int client_count = 0;
	while (1)
	{
		client_addr_len = sizeof(client_addr);
		
		int client = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);

		if (client < 0){
			exit(1);
		}
		printf("Client connected: %d\n", ++client_count);
		if ((childpid = fork()) == 0) {
			struct HttpRequest request;
			char req_buffer[BUFFER_SIZE] = {0};
			int bytes_received = recv(client, req_buffer, sizeof(req_buffer), 0);

			printf("%s\n", req_buffer);
			char *directory_path = getDirectoryPath(argc, argv);
			initHttpRequest(&request, req_buffer);
			processResponse(&request, client, directory_path);
		}
	}

	close(server_fd);

	return 0;
}


void initHttpRequest(struct HttpRequest *request, char *req_buffer) {


	char *token = NULL;
	char *rest = strdup(req_buffer);
	char *req_body[3] = {NULL};
	int i = 0;

	initHeader(request->user_agent, sizeof(request->user_agent), strdup(req_buffer), "User-Agent");
	initHeader(request->accepted_encoding	, sizeof(request->accepted_encoding), strdup(req_buffer), "Accept-Encoding");

	while ((token = strtok_r(rest, " ", &rest))){
		req_body[i++] = token;
		if (i >= 3)
			break;
	}


    strncpy(request->method, req_body[0], sizeof(request->method)-1);
    strncpy(request->path, req_body[1], sizeof(request->path)-1);
    strncpy(request->protocol, req_body[2], sizeof(request->protocol)-1);
    request->method[sizeof(request->method)-1] = '\0';
    request->path[sizeof(request->path)-1] = '\0';
    request->protocol[sizeof(request->protocol)-1] = '\0';

	char *body = strstr(strdup(req_buffer), "\r\n\r\n");
	
	if (body != NULL){
		body += 4;
		strncpy(request->body, body, sizeof(request->body)-1);
		request->body[sizeof(request->body)-1] = '\0';
	}
	else{
		request->body[0] = '\0';
	}
	
}

void initHttpResponse(struct HttpResponse *response, char *status, char *content_encoding, char *body) {

    strncpy(response->status, status, sizeof(response->status) - 1);
    strncpy(response->content_encoding, content_encoding, sizeof(response->content_encoding) - 1);
    strncpy(response->body, body, sizeof(response->body) - 1);

    response->status[sizeof(response->status) - 1] = '\0';
    response->content_encoding[sizeof(response->content_encoding) - 1] = '\0';
    response->body[sizeof(response->body) - 1] = '\0';

}

void initHeader(char *header_value, size_t header_value_size, char *req_buffer, const char *header_name) {
    char *token = NULL;
    char *rest = req_buffer;
    char *line = NULL;
    char *header[2] = {NULL};

    while ((line = strtok_r(rest, "\r\n", &rest))) {
        token = strtok(line, ":");
        if (token) {
            header[0] = token;
            token = strtok(NULL, "");
            if (token) {
                header[1] = token;
            } else {
                header[1] = "";
            }

            if (strcmp(header[0], header_name) == 0) {
                strncpy(header_value, header[1], header_value_size - 1);
                header_value[header_value_size - 1] = '\0';
                return;
            }
        }
    }

    strncpy(header_value, "no-value", header_value_size - 1);
    header_value[header_value_size - 1] = '\0';

	for (int i = 0; i < strlen(header_value); i++) {
        header_value[i] = tolower(header_value[i]);
    }

}


//http request header에서 GET,POST 인지를 감지해서 처리
void processResponse(struct HttpRequest *request, int client, char *directory){
	
	int path = strcmp(request->path, "/");
	
	if (strcmp(request->method, "GET") == 0)
	{

		if (path == 0){
			char buffer_resp_200[BUFFER_SIZE];
			sprintf(buffer_resp_200, "%s\r\n", resp_200);
			send(client, buffer_resp_200, strlen(buffer_resp_200),0);
			return;
		}
        //GET /echo/hello HTTP/1.1 인 경우, "hello" 를 서버에 echo 하는 코드
		else if (serverEcho(request, client) != 1 ){
			if (requestUserAgent(request, client) != 1){
				if (requestFile(request, client, directory) != 1){
					send(client, resp_404, strlen(resp_404),0);
				}
			}
		}
		else{
			send(client, resp_404, strlen(resp_404),0);
		}
	
	}
	else if (strcmp(request->method, "POST") == 0)
	{
		printf("POST METHOD....");
		if (postFile(request, client, directory) != 1){
			printf("NO DATA CONTENT");
			send(client, resp_404, strlen(resp_404),0);
		}
	}
}

int serverEcho(struct HttpRequest *request, int client) {
    char *token = NULL;
    char *rest = strdup(request->path);
    char *echo_path[2] = {NULL};
    int i = 0;
    while ((token = strtok_r(rest, "/", &rest))){
        echo_path[i++] = token;
        if (i >= 2)
            break;
    }

    int echo = strcmp(echo_path[0], "echo");

    if (echo == 0) {
        struct HttpResponse response;
        initHttpResponse(&response, resp_200, "", echo_path[1]);

        printf(".....%s\n", response.body);

        int count = 0;
        char **accepted_encodings = tokenizer(request->accepted_encoding, ", ", &count);

        if (checkIfExistsInArray(accepted_encodings, count, server_accepted_encoding) == 1) {
            printf("ENCODING...FOUND...%s\n", request->accepted_encoding);
            strcpy(response.content_encoding, server_accepted_encoding);

            const char *input = request->body;
            size_t input_size = strlen(input);
            unsigned char *compressed_output = NULL;
            size_t compressed_size = 0;

            // Compress the input
            int ret = gzip_compress(input, input_size, &compressed_output, &compressed_size);
            if (ret != Z_OK) {
                fprintf(stderr, "GZIP compression failed: %d\n", ret);
                freeTokens(accepted_encodings);
                return 1;
            }

            printf("Input size: %zu bytes\n", input_size);
            printf("Compressed size: %zu bytes\n", compressed_size);

            // Copy compressed data to response body
            memcpy(response.body, compressed_output, compressed_size);
            response.body[compressed_size] = '\0';  // Null terminate

            char *echo_response = writeResponse("application/gzip", &response, compressed_size);

            if (echo_response != NULL) {
                send(client, echo_response, strlen(echo_response), 0);
                free(echo_response);
            }

            free(compressed_output);
            freeTokens(accepted_encodings);
            return 1; // server echo success
        }
    }

    return 0; // server echo 404
}


int requestUserAgent(struct HttpRequest *request, int client){
	char *token = NULL;
	char *rest = strdup(request->path);
	char *user_agent_path[1] = {NULL};
	int i = 0;
	while ((token = strtok_r(rest, "/", &rest))){
		user_agent_path[i++] = token;
		if (i >= 1)
			break;
	}

	int user_agent = strcmp(user_agent_path[0], "user-agent");
	if (user_agent == 0){

		struct HttpResponse response;
		initHttpResponse(&response, resp_200, "", request->user_agent);

		char *user_agent_response = writeResponse("text/plain", &response, strlen(response.body));

		if (user_agent_response != NULL){
			send(client, user_agent_response, strlen(user_agent_response),0);
			free(user_agent_response);
		}
		else{
			send(client, resp_404, strlen(resp_404),0);
		}

		return 1;
	}

	return 0;
}

int requestFile(struct HttpRequest *request, int client, char *directory){
	char *token = NULL;
	char *rest = strdup(request->path);
	char *file_path[2] = {NULL};
	int i = 0;
	
	while ((token = strtok_r(rest, "/", &rest))){
		file_path[i++] = token;
		if (i >= 2)
			break;
	}

	int file = strcmp(file_path[0], "files");
	if (file == 0 && i >= 2){

		char *dir_file = (char *)malloc(BUFFER_SIZE);
		snprintf(dir_file, BUFFER_SIZE, "%s/%s", directory, file_path[1]);
		printf("directory %s\n", dir_file);

		struct HttpResponse response;
		initHttpResponse(&response, resp_200, "", dir_file);

		char *file_response = writeResponse("application/octet-stream", &response, strlen(response.body));
		free(dir_file);
		if (file_response != NULL){
			send(client, file_response, strlen(file_response),0);
			free(file_response);
		}
		else{
			send(client, resp_404, strlen(resp_404),0);
		}

		return 1;
	}

	return 0;
}

int postFile(struct HttpRequest *request, int client, char *directory){
	char *token = NULL;
	char *rest = strdup(request->path);
	char *file_path[2] = {NULL};
	int i = 0;
	
	while ((token = strtok_r(rest, "/", &rest))){
		file_path[i++] = token;
		if (i >= 2)
			break;
	}

	int file = strcmp(file_path[0], "files");
	if (file == 0 && i >= 2){

		char *dir_file = (char *)malloc(BUFFER_SIZE);
		snprintf(dir_file, BUFFER_SIZE, "%s/%s", directory, file_path[1]);
		printf(" POST FILE IN DIR %s\n", dir_file);

		if (request->body[0] != '\0'){
			printf("WRITING.... TO %s\n", dir_file);
			writeFile(dir_file, request->body);
			send(client, resp_201, strlen(resp_201),0);

			return 1;
		}
		
	}

	return 0;
}

char *writeResponse(char *type, struct HttpResponse *response, size_t body_len) {
    if (response->body == NULL) {
        return NULL;
    }

    char *buffer = (char *)malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        printf("SERVER ERROR");
        return NULL;
    }

    trimString(response->body);

    size_t len = 0;

    if (strcmp(type, "text/plain") == 0 || strcmp(type, "application/gzip") == 0) {
        if (body_len > 0) {
            if (response->content_encoding[0] != '\0') {
                len = snprintf(buffer, BUFFER_SIZE, "%sContent-Encoding: %s\r\nContent-Type: %s\r\nContent-Length: %zu\r\n\r\n", response->status, response->content_encoding, type, body_len);
            } else {
                len = snprintf(buffer, BUFFER_SIZE, "%sContent-Type: %s\r\nContent-Length: %zu\r\n\r\n", response->status, type, body_len);
            }

            if (len < 0 || len >= BUFFER_SIZE) {
                printf("SERVER ERROR");
                free(buffer);
                return NULL;
            }

            memcpy(buffer + len, response->body, body_len);
            len += body_len;
        }
    }

    return buffer;
}

char *readFile(char *filename){
	FILE *file = fopen(filename, "rb");
	
	if (file == NULL){
		fprintf(stderr, "Error opening file %s or it does not exist", filename);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *buffer = malloc(file_size+1);
	if (buffer == NULL){
		fprintf(stderr, "Error allocating memory for file %s", filename);
		fclose(file);
		return NULL;
	}

	size_t bytes_read = fread(buffer, 1, file_size, file);
	if (bytes_read != file_size){
		fprintf(stderr, "Error reading file %s, probably file too large to be processed", filename);
		fclose(file);
		free(buffer);
		return NULL;
	}

	buffer[file_size] = '\0';

	fclose(file);

	return buffer;

}

void writeFile(char *filename, char *content){
	FILE *file = fopen(filename, "a");

	if (file == NULL) {
        fprintf(stderr, "Error opening file %s", filename);
        return;
    }

	if (fprintf(file, "%s", content) < 0) {
        fprintf(stderr, "Error writing to file %s", filename);
    }

	fclose(file);
}

char *getDirectoryPath(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--directory") == 0) {
            if (i + 1 < argc) {
                return argv[i + 1];
            } else {
                fprintf(stderr, "--directory requires a value\n");
                return NULL;
            }
        }
    }
    return NULL;
}

int checkIfExistsInArray(char *origin_a[], int origin_a_len, char *value)
{
	for (int i = 0; i < origin_a_len; i++){
		if (strcmp(value, origin_a[i]) == 0){
			return 1;
		}
	}

	return 0;
}

char** tokenizer(const char* str, const char* delim, int* count) {
    char* str_copy = strdup(str);
    if (str_copy == NULL) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    int token_count = 0;
    char* token = strtok(str_copy, delim);
    while (token != NULL) {
        token_count++;
        token = strtok(NULL, delim);
    }

    char** tokens = malloc((token_count + 1) * sizeof(char*));
    if (tokens == NULL) {
        perror("malloc");
        free(str_copy);
        exit(EXIT_FAILURE);
    }

    strcpy(str_copy, str);

    token_count = 0;
    token = strtok(str_copy, delim);
    while (token != NULL) {
        tokens[token_count] = strdup(token);
        if (tokens[token_count] == NULL) {
            perror("strdup");
            for (int i = 0; i < token_count; i++) {
                free(tokens[i]);
            }
            free(tokens);
            free(str_copy);
            exit(EXIT_FAILURE);
        }
        token_count++;
        token = strtok(NULL, delim);
    }
    tokens[token_count] = NULL;

    free(str_copy);

    if (count != NULL) {
        *count = token_count;
    }

    return tokens;
}

void freeTokens(char** tokens) {
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

void trimString(char *str) {

  char *token = strtok(str, " \t\n\r");

  if (token)

    strcpy(str, token);

}

int gzip_compress(const char *input, size_t input_size, unsigned char **output, size_t *output_size) {
    z_stream strm;
    int ret;
    unsigned char out_buffer[8192];

    *output = NULL;
    *output_size = 0;

    memset(&strm, 0, sizeof(strm));
    ret = deflateInit2(&strm, Z_BEST_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        return ret;
    }

    strm.next_in = (unsigned char *)input;
    strm.avail_in = input_size;

    do {
        strm.next_out = out_buffer;
        strm.avail_out = sizeof(out_buffer);

        ret = deflate(&strm, Z_FINISH);
        if (ret != Z_STREAM_END && ret != Z_OK && ret != Z_BUF_ERROR) {
            deflateEnd(&strm);
            return ret;
        }

        size_t have = sizeof(out_buffer) - strm.avail_out;
        *output = realloc(*output, *output_size + have);
        if (*output == NULL) {
            deflateEnd(&strm);
            return Z_MEM_ERROR;
        }

        memcpy(*output + *output_size, out_buffer, have);
        *output_size += have;
    } while (strm.avail_out == 0);

    deflateEnd(&strm);
    return Z_OK;
}


void print_bytes(char *by, size_t len) {

  unsigned char *bytes = (unsigned char *)by;
  printf("[");
  for (size_t i = 0; i < len; ++i) {
    printf("%02x", bytes[i]);
    if (i < len - 1) {
      printf(", ");
    }
  }
  printf("]");

}