#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Test utilities
#define TEST_PORT 8080
#define BUFFER_SIZE 4096
#define ASSERT_STR_EQ(a, b) assert(strcmp(a, b) == 0)
#define ASSERT_INT_EQ(a, b) assert(a == b)

// HTTP request utilities
typedef struct {
    char* response;
    size_t length;
} http_response;

// Function to make HTTP request
http_response make_request(const char* method, const char* path) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TEST_PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    
    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    char request[BUFFER_SIZE];
    snprintf(request, BUFFER_SIZE, 
             "%s %s HTTP/1.1\r\n"
             "Host: localhost:%d\r\n"
             "\r\n", 
             method, path, TEST_PORT);
    
    send(sock, request, strlen(request), 0);
    
    http_response resp;
    resp.response = malloc(BUFFER_SIZE);
    resp.length = recv(sock, resp.response, BUFFER_SIZE - 1, 0);
    resp.response[resp.length] = '\0';
    
    close(sock);
    return resp;
}

// Function to extract JSON body from HTTP response
char* get_json_body(http_response resp) {
    char* body = strstr(resp.response, "\r\n\r\n");
    return body ? body + 4 : NULL;
}

// Test cases

// Test 1: New game creation
void test_new_game() {
    printf("Running test_new_game...\n");
    
    http_response resp = make_request("GET", "/new_game");
    char* body = get_json_body(resp);
    assert(body != NULL);
    
    // Check if response contains required fields
    assert(strstr(body, "\"game_id\"") != NULL);
    assert(strstr(body, "\"board\"") != NULL);
    assert(strstr(body, "\"white_to_move\":true") != NULL);
    
    // Verify initial FEN string
    assert(strstr(body, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR") != NULL);
    
    free(resp.response);
    printf("test_new_game passed!\n");
}

// Test 2: Get game state
void test_get_game() {
    printf("Running test_get_game...\n");
    
    // First create a new game
    http_response new_game = make_request("GET", "/new_game");
    char* new_game_body = get_json_body(new_game);
    
    // Extract game_id from response
    char game_id[10];
    char* id_start = strstr(new_game_body, "\"game_id\":") + 9;
    sscanf(id_start, "%[^,}]", game_id);
    
    // Create path for game state request
    char path[50];
    snprintf(path, sizeof(path), "/game/%s", game_id);
    
    // Get game state
    http_response resp = make_request("GET", path);
    char* body = get_json_body(resp);
    
    // Verify it's the same game
    assert(strstr(body, game_id) != NULL);
    
    free(new_game.response);
    free(resp.response);
    printf("test_get_game passed!\n");
}

// Test 3: Invalid game ID
void test_invalid_game() {
    printf("Running test_invalid_game...\n");
    
    http_response resp = make_request("GET", "/game/99999");
    char* body = get_json_body(resp);
    
    assert(strstr(body, "error") != NULL);
    
    free(resp.response);
    printf("test_invalid_game passed!\n");
}

// Test 4: Initial board state verification
void test_initial_board() {
    printf("Running test_initial_board...\n");
    
    http_response resp = make_request("GET", "/new_game");
    char* body = get_json_body(resp);
    
    // Verify white pieces
    assert(strstr(body, "RNBQKBNR") != NULL);  // Back rank
    assert(strstr(body, "PPPPPPPP") != NULL);  // Pawns
    
    // Verify black pieces
    assert(strstr(body, "rnbqkbnr") != NULL);  // Back rank
    assert(strstr(body, "pppppppp") != NULL);  // Pawns
    
    free(resp.response);
    printf("test_initial_board passed!\n");
}

// Main test runner
int main() {
    printf("Starting chess server tests...\n\n");
    
    // Run all tests
    test_new_game();
    test_get_game();
    test_invalid_game();
    test_initial_board();
    
    printf("\nAll tests passed successfully!\n");
    return 0;
}
