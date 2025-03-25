// standard lib
#include <stdio.h> //printf(), error(), perror()
#include <stdlib.h> //#define EXIT_FAILURE
#include <string.h> //strlen(), etc 
#include <errno.h> //error related 

// file related 
#include <unistd.h>
#include <fcntl.h> //file descriptor (read, write flag) 

// socket related 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// 비동기적 처리 related 
#include <pthread.h>
#include <sys/event.h>
#include <ctype.h>


//server config
#define PORT 8080
#define MAX_EVENTS 64
#define MAX_WORKERS 4

//chess config
#define BUFFER_SIZE 4096
#define MAX_GAMES 100
#define BOARD_SIZE 64
#define FEN_MAX_LENGTH 100


// Chess piece representations
typedef enum {
    EMPTY = 0,
    WHITE_PAWN = 1,
    WHITE_KNIGHT = 2,
    WHITE_BISHOP = 3,
    WHITE_ROOK = 4,
    WHITE_QUEEN = 5,
    WHITE_KING = 6,
    BLACK_PAWN = -1,
    BLACK_KNIGHT = -2,
    BLACK_BISHOP = -3,
    BLACK_ROOK = -4,
    BLACK_QUEEN = -5,
    BLACK_KING = -6
} ChessPiece;

// Game state structure
typedef struct {
    int game_id;
    ChessPiece board[BOARD_SIZE];
    int white_to_move;
    char castling_rights[5];  // "KQkq" + null terminator
    char en_passant[3];       // Square in algebraic notation
    int halfmove_clock;
    int fullmove_number;
    char last_fen[FEN_MAX_LENGTH];
} GameState;

// Global games array
GameState games[MAX_GAMES];
int next_game_id = 1;
pthread_mutex_t games_mutex = PTHREAD_MUTEX_INITIALIZER;


// Operation types (similar to IOCP)
typedef enum {
    OP_ACCEPT,
    OP_READ,
    OP_WRITE
} operation_type;

// Overlapped structure (similar to IOCP OVERLAPPED)
typedef struct {
    operation_type op_type;
    void* buffer;
    size_t buffer_len;
    int socket_fd;
    void (*callback)(struct kevent*);
} overlapped_t;

// Completion Port structure
typedef struct {
    int kqueue_fd;
    pthread_t worker_threads[MAX_WORKERS];
    int running;
} completion_port_t;

// Global variables
completion_port_t* g_iocp = NULL;
int g_server_fd;



// Function to parse FEN string into board state
int parse_fen(const char* fen, GameState* game) {
    int rank = 7;  // Start from 8th rank (0-based)
    int file = 0;
    int index = 0;
    
    // Clear the board first
    memset(game->board, 0, sizeof(game->board));
    
    // Parse piece placement
    while (rank >= 0 && *fen) {
        if (*fen == '/') {
            rank--;
            file = 0;
        } else if (isdigit(*fen)) {
            file += (*fen - '0');
        } else {
            int square = rank * 8 + file;
            switch (*fen) {
                case 'P': game->board[square] = WHITE_PAWN; break;
                case 'N': game->board[square] = WHITE_KNIGHT; break;
                case 'B': game->board[square] = WHITE_BISHOP; break;
                case 'R': game->board[square] = WHITE_ROOK; break;
                case 'Q': game->board[square] = WHITE_QUEEN; break;
                case 'K': game->board[square] = WHITE_KING; break;
                case 'p': game->board[square] = BLACK_PAWN; break;
                case 'n': game->board[square] = BLACK_KNIGHT; break;
                case 'b': game->board[square] = BLACK_BISHOP; break;
                case 'r': game->board[square] = BLACK_ROOK; break;
                case 'q': game->board[square] = BLACK_QUEEN; break;
                case 'k': game->board[square] = BLACK_KING; break;
                default: return 0;  // Invalid character
            }
            file++;
        }
        fen++;
    }
    
    // Store the FEN string
    strncpy(game->last_fen, fen, FEN_MAX_LENGTH - 1);
    game->last_fen[FEN_MAX_LENGTH - 1] = '\0';
    
    return 1;  // Success
}

// Function to create game state string (simplified JSON)
void create_game_state_json(GameState* game, char* buffer, size_t buffer_size) {
    char board_str[BOARD_SIZE * 2 + 1] = {0};
    int i;
    
    // Convert board to string representation
    for (i = 0; i < BOARD_SIZE; i++) {
        char piece = '.';
        switch (game->board[i]) {
            case WHITE_PAWN: piece = 'P'; break;
            case WHITE_KNIGHT: piece = 'N'; break;
            case WHITE_BISHOP: piece = 'B'; break;
            case WHITE_ROOK: piece = 'R'; break;
            case WHITE_QUEEN: piece = 'Q'; break;
            case WHITE_KING: piece = 'K'; break;
            case BLACK_PAWN: piece = 'p'; break;
            case BLACK_KNIGHT: piece = 'n'; break;
            case BLACK_BISHOP: piece = 'b'; break;
            case BLACK_ROOK: piece = 'r'; break;
            case BLACK_QUEEN: piece = 'q'; break;
            case BLACK_KING: piece = 'k'; break;
        }
        board_str[i * 2] = piece;
        board_str[i * 2 + 1] = ' ';
    }
    board_str[BOARD_SIZE * 2 - 1] = '\0';
    
    snprintf(buffer, buffer_size,
             "{"
             "\"game_id\":%d,"
             "\"board\":\"%s\","
             "\"white_to_move\":%s,"
             "\"fen\":\"%s\""
             "}",
             game->game_id,
             board_str,
             game->white_to_move ? "true" : "false",
             game->last_fen);
}

// Function to handle chess game requests
void handle_chess_request(const char* request, char* response, size_t response_size) {
    if (strncmp(request, "GET /new_game", 12) == 0) {
        // Create new game with initial position
        pthread_mutex_lock(&games_mutex);
        int game_id = next_game_id++;
        GameState* game = &games[game_id % MAX_GAMES];
        game->game_id = game_id;
        parse_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", game);
        create_game_state_json(game, response, response_size);
        pthread_mutex_unlock(&games_mutex);
    }
    else if (strncmp(request, "POST /move/", 10) == 0) {
        // Handle move - Parse game_id and move from request
        // (Simplified - actual implementation would need proper move validation)
        response[0] = '{';
        response[1] = '}';
        response[2] = '\0';
    }
    else if (strncmp(request, "GET /game/", 9) == 0) {
        // Get game state
        int game_id;
        sscanf(request + 9, "%d", &game_id);
        pthread_mutex_lock(&games_mutex);
        GameState* game = &games[game_id % MAX_GAMES];
        if (game->game_id == game_id) {
            create_game_state_json(game, response, response_size);
        } else {
            snprintf(response, response_size, "{\"error\":\"Game not found\"}");
        }
        pthread_mutex_unlock(&games_mutex);
    }
    else {
        snprintf(response, response_size, "{\"error\":\"Invalid request\"}");
    }
}

// HTTP response handler
void handle_http_request(int client_fd, const char* request) {
    char* response_body = malloc(4096);
    handle_chess_request(request, response_body, 4096);

    // HTTP 응답 생성
    char* response = malloc(8192);
    snprintf(response, 8192,
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: %zu\r\n"
                "\r\n"
                "%s",
                strlen(response_body), response_body);
    
    // Write 작업 시작
    post_write(client_fd, response, strlen(response));
    
    free(response_body);
    free(response);
    
    // char response_body[4096];
    // handle_chess_request(request, response_body, sizeof(response_body));

    // // Create HTTP response
    // char response[8192];
    // snprintf(response, sizeof(response),
    //          "HTTP/1.1 200 OK\r\n"
    //          "Content-Type: application/json\r\n"
    //          "Content-Length: %zu\r\n"
    //          "\r\n"
    //          "%s",
    //          strlen(response_body), response_body);
    
    // send(client_fd, response, strlen(response), 0);
}


/**************************
 * async code from 06.http_server/step5.async_io
 */

// Initialize completion port
completion_port_t* create_completion_port() {
    completion_port_t* iocp = malloc(sizeof(completion_port_t));
    iocp->kqueue_fd = kqueue();
    if (iocp->kqueue_fd == -1) {
        free(iocp);
        return NULL;
    }
    iocp->running = 1;
    return iocp;
} 

// Worker thread function
void* worker_thread(void* arg) {
    struct kevent events[MAX_EVENTS];
    
    while (g_iocp->running) {
        int nev = kevent(g_iocp->kqueue_fd, NULL, 0, 
                        events, MAX_EVENTS, NULL);
        
        for (int i = 0; i < nev; i++) {
            overlapped_t* overlapped = events[i].udata;
            if (overlapped && overlapped->callback) {
                overlapped->callback(&events[i]);
            }
        }
    }
    
    return NULL;
}

// Read 완료 핸들러
void handle_read(struct kevent* event) {
    overlapped_t* overlapped = (overlapped_t*)event->udata;
    char* buffer = overlapped->buffer;
    int client_fd = overlapped->socket_fd;
    
    if (event->data > 0) {  // 데이터 읽기 성공
        // HTTP 요청 처리
        handle_http_request(client_fd, buffer);
        
        // 응답 후 정리
        free(buffer);
        free(overlapped);
    } else {
        // 연결 종료 또는 에러
        close(client_fd);
        free(buffer);
        free(overlapped);
    }
}

// Start async read
void post_read(int socket_fd) {
    overlapped_t* overlapped = malloc(sizeof(overlapped_t));
    overlapped->op_type = OP_READ;
    overlapped->socket_fd = socket_fd;
    overlapped->buffer = malloc(BUFFER_SIZE);
    overlapped->buffer_len = BUFFER_SIZE;
    overlapped->callback = handle_read; //async 비동기 처리는 필드에 ->callback 추가해서 함수를 넘겨주는구나 
    
    struct kevent event;
    EV_SET(&event, socket_fd, EVFILT_READ,  //비동기 처리를 여기서 하는 덧 
           EV_ADD | EV_ENABLE | EV_ONESHOT, 
           0, 0, overlapped);
    
    kevent(g_iocp->kqueue_fd, &event, 1, NULL, 0, NULL);
}

// Handle completed accept
void handle_accept(struct kevent* event) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    int client_fd = accept(g_server_fd, 
                          (struct sockaddr*)&client_addr, 
                          &client_len);
    
    if (client_fd != -1) {
        // Make client socket non-blocking
        int flags = fcntl(client_fd, F_GETFL, 0);
        fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
        
        // Start reading from client
        post_read(client_fd);
    }
    
    // Continue accepting next request
    post_accept();
}

// Start async accept
void post_accept() {
    overlapped_t* overlapped = malloc(sizeof(overlapped_t));
    overlapped->op_type = OP_ACCEPT;
    overlapped->socket_fd = g_server_fd;
    overlapped->callback = handle_accept; //Q. error. 기존 handle_accept() from step4 takes kevent* event but handle_http_request() takes another parameters 
    
    struct kevent event;
    EV_SET(&event, g_server_fd, EVFILT_READ, 
           EV_ADD | EV_ENABLE, 0, 0, overlapped);
    kevent(g_iocp->kqueue_fd, &event, 1, NULL, 0, NULL);
}

int main() {
    // Create server socket
    g_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (g_server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Set socket options
    int opt = 1;
    setsockopt(g_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // Make server socket non-blocking
    int flags = fcntl(g_server_fd, F_GETFL, 0);
    fcntl(g_server_fd, F_SETFL, flags | O_NONBLOCK);
    
    // Bind and listen
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if (bind(g_server_fd, (struct sockaddr*)&server_addr, 
             sizeof(server_addr)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(g_server_fd, SOMAXCONN) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    
    // Initialize completion port
    g_iocp = create_completion_port();
    if (!g_iocp) {
        perror("create_completion_port failed");
        exit(EXIT_FAILURE);
    }
    
    // Start worker threads
    for (int i = 0; i < MAX_WORKERS; i++) {
        pthread_create(&g_iocp->worker_threads[i], NULL, 
                      worker_thread, NULL);
    }
    
    // Start accepting connections (accept http_request from clients)
    post_accept();
    
    printf("Server running on port %d\n", PORT);
    
    // Wait for worker threads
    for (int i = 0; i < MAX_WORKERS; i++) {
        pthread_join(g_iocp->worker_threads[i], NULL);
    }
    
    return 0;
}
