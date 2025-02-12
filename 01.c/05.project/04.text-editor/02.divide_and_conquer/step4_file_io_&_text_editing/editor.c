#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>

/*
Q. whats added?

1. text 입력
    - char 입력, backspace to delete, enter
    - Q. while문으로 char input 받아서 E.erow에 write()하나?
2. save & load (ctrl + s)
    - Q. dirty read? 어떻게 구현되어있지?
        - editorInsertChar() -> E.dirty = 1 -> 문자 내용 입력 후, "변경됨" 상태 설정
        - editorRowDelChar() -> E.dirty = 1 -> 문자 내용 삭제 후, "변경됨" 상태 설정
        - editorOpen() -> E.dirty = 1 -> 파일 방금 읽어왔으므로, 변경되지 않은 상태
        - editorSave() -> E.dirty = 0 -> 성공적으로 저장했으므로, 변경되지 않은 상태 
    - Q. 이거 왜함?
        - A. 파일 종료 전에 저장하지 않은 변경사항 있으면 경고띄움. 마치, vim에서 edit하고 :q 누르면 변경사항 있다고 :sq 하라고 경고뜨는 것 처럼.
3. scrolling
    - Q. scrolling 어떻게 구현되어있지?
    - arrow key to move cursor around -> adjust E.cx, E.cy
*/


/* Defines */
#define CTRL_KEY(k) ((k) & 0x1f) // Ctrl 키 조합을 처리하기 위한 매크로
#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE // getline() 함수 사용을 위한 GNU 확장 기능 활성화

/* Enums and Data structures */
enum editorKey {
    ENTER = 13,
    BACKSPACE = 127,
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN
};

typedef struct erow {
    int size;
    char *chars;
} erow;

struct editorConfig {
    int cx, cy;          // 커서의 x,y 좌표 
    int rowoff;          // 수직 스크롤을 위한 offset
    int coloff;          // New!: 수평 스크롤을 위한 offset
    int screenrows;      // window size y 
    int screencols;      // window size x
    int numrows;         // 총 쓴 rows 수 
    erow *row;           // 각 row안에 있는 char들과 their size
    char *filename;      // New!: Currently open file
    int dirty;           // New!: File modified flag
    struct termios orig_termios;
};

struct editorConfig E;

/* Function declarations */
void editorAppendRow(char *s, size_t len);
void editorMoveCursor(int key);
int editorReadKey(void);
void enableRawMode(void);
void initEditor(void);
char *editorRowsToString(int *buflen);
void die(const char *s);
void editorRefreshScreen(void);
void editorProcessKeypress(void);
int getWindowSize(int *rows, int *cols);
void editorScroll(void);
void editorDrawRows(void);
void editorSave(void);
void editorRowAppendString(erow *row, char *s, size_t len);
void editorDelRow(int at);


/* Terminal handling */
void die(const char *s) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    perror(s);
    exit(1);
}

int getWindowSize(int *rows, int *cols) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        return -1;
    } else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
        die("tcsetattr");
}

//from step1
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    
    //Q. 어떤 flag option들이 추가됬지? diff from step1?
    struct termios raw = E.orig_termios;

    // Modify input flags (~: bitwise NOT)
    raw.c_iflag &= ~(
        ICRNL  |    // Disable CR to NL translation
        IXON   |    // Disable software flow control (Ctrl-S, Ctrl-Q)
        BRKINT |    // New!: BREAK 신호를 받았을 때 SIGINT 발생을 방지
        INPCK  |    // New!: 패리티 검사 비활성화
        ISTRIP      // New!: 8번째 비트 스트립 비활성화 (8비트 문자 처리를 위해)
    );
    
    // Modify output flags
    raw.c_oflag &= ~(
        OPOST      // Disable all output processing
    );
    
    // New!: control flags
    raw.c_cflag |= (CS8); // 문자 크기를 8비트로 설정

    // New!: Control chars
    raw.c_cc[VMIN] = 0;   // read()가 반환하기 위한 최소 바이트 수
    raw.c_cc[VTIME] = 1;  // read() 타임아웃 시간을 0.1초로 설정

    // Modify local flags
    raw.c_lflag &= ~(
        ECHO |     // Disable echoing of input characters
        ICANON |   // Disable canonical mode (line-by-line input)
        IEXTEN |   // Disable extended input processing
        ISIG       // Disable signal generation (Ctrl-C, Ctrl-Z)
    );
    
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

/* Row operations */
void editorAppendRow(char *s, size_t len) {
    E.row = realloc(E.row, sizeof(erow) * (E.numrows + 1)); //이 때 realloc()을 쓰는구나! 기존에 있던 메모리 공간을 변경하고 싶을 때.
    
    int at = E.numrows;
    E.row[at].size = len;
    E.row[at].chars = malloc(len + 1);
    memcpy(E.row[at].chars, s, len);
    E.row[at].chars[len] = '\0';
    E.numrows++;
    E.dirty = 1; //dirty write
}

// Append string to an existing row
void editorRowAppendString(erow *row, char *s, size_t len) {
    row->chars = realloc(row->chars, row->size + len + 1);
    memcpy(&row->chars[row->size], s, len);
    row->size += len;
    row->chars[row->size] = '\0';
    E.dirty = 1;
}

void editorRowInsertChar(erow *row, int at, int c) {
    if (at < 0 || at > row->size) at = row->size;
    row->chars = realloc(row->chars, row->size + 2); //char을 입력하기 위해 더 많은 공간 할당받아야 하니까, realloc() 쓴다.
    memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1); //메모리를 복사하여 이동한다. 
    row->size++;
    row->chars[at] = c;
    E.dirty = 1;
}

//backspace to delete 
void editorRowDelChar(erow *row, int at) {
    if (at < 0 || at >= row->size) return;
    memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
    row->size--;
    E.dirty = 1;
}

// Delete a row by backspace
void editorDelRow(int at) {
    if (at < 0 || at >= E.numrows) return;
    free(E.row[at].chars);
    memmove(&E.row[at], &E.row[at + 1], sizeof(erow) * (E.numrows - at - 1));
    E.numrows--;
    E.dirty = 1;
}

//enter key -> new line
void editorInsertNewline() {
    // If cursor is at the start of the line, just insert a new empty line
    if (E.cx == 0) {
        editorAppendRow("", 0);
    } else {
        // Get current row
        erow *row = &E.row[E.cy];
        
        // Split the line at cursor position
        // Create new row with the content after cursor
        editorAppendRow(&row->chars[E.cx], row->size - E.cx);
        
        // Update the current row to contain only the content before cursor
        row = &E.row[E.cy];  // Re-get the pointer as editorAppendRow might have reallocated memory
        row->size = E.cx;
        row->chars[row->size] = '\0';
    }
    E.cy++;    // Move to next line
    E.cx = 0;  // Move cursor to start of line
    E.dirty = 1;
}

/* Editor operations */
void editorInsertChar(int c) { //int c은 사실상 char c인데, int type으로 표현한 것 
    //if there is no text at all, 
    if (E.cy == E.numrows) {
        editorAppendRow("", 0); //새 row 생성 
    }

    //현재 커서 위치에 문자 삽입 
    editorRowInsertChar(&E.row[E.cy], E.cx, c); //int c은 사실상 char c인데, int type으로 표현한 것 
    E.cx++;
}

void editorDelChar() {
    // If cursor is beyond file content or at the very beginning of file, return
    if (E.cy == E.numrows) return;
    if (E.cx == 0 && E.cy == 0) return;

    erow *row = &E.row[E.cy];

    //delete on current line
    if (E.cx > 0) {
        editorRowDelChar(row, E.cx - 1);
        E.cx--;
    } else { //delete beyond current line: move cursor to a line before and keep deleting
       // At beginning of line, need to append this line to the end of previous line
       E.cx = E.row[E.cy-1].size;  // Move cursor to end of previous line
       editorRowAppendString(&E.row[E.cy-1], row->chars, row->size);  // Append current line to previous line
       editorDelRow(E.cy);  // Delete current line
       E.cy--;  // Move cursor up to previous line 
    }
}

/* Screen handling */
void editorScroll() {
    //스크롤이 화면 밖이면, 
    if (E.cy < E.rowoff) {
        //adjust y시작값
        E.rowoff = E.cy;
    }
    //터미널 화면y값이 스크롤한 정도 + 스크롤한 이후 y 시작값보다 작으면?
    if (E.cy >= E.rowoff + E.screenrows) {
        //adjust y시작값
        E.rowoff = E.cy - E.screenrows + 1;
    }

    //x축도 너무 글을 길게 쓰면 화면 바깥으로 튀어나오는 경우가 있는데, 그걸 자르는 목적으로 씀
    if (E.cx < E.coloff) {
        E.coloff = E.cx;
    }
    if (E.cx >= E.coloff + E.screencols) {
        E.coloff = E.cx - E.screencols + 1;
    }
}

//same from step3
void editorDrawRows() {
    for (int y = 0; y < E.screenrows; y++) {
        int filerow = y + E.rowoff;
        if (filerow >= E.numrows) {
            if (E.numrows == 0 && y == E.screenrows / 3) {
                char welcome[80];
                int welcomelen = snprintf(welcome, sizeof(welcome),
                    "Mini editor -- version 0.0.1");
                if (welcomelen > E.screencols) welcomelen = E.screencols;
                int padding = (E.screencols - welcomelen) / 2;
                if (padding) {
                    write(STDOUT_FILENO, "~", 1);
                    padding--;
                }
                while (padding--) write(STDOUT_FILENO, " ", 1);
                write(STDOUT_FILENO, welcome, welcomelen);
            } else {
                write(STDOUT_FILENO, "~", 1);
            }
        } else {
            int len = E.row[filerow].size - E.coloff;
            if (len < 0) len = 0;
            if (len > E.screencols) len = E.screencols;
            write(STDOUT_FILENO, &E.row[filerow].chars[E.coloff], len);
        }

        write(STDOUT_FILENO, "\x1b[K", 3);
        if (y < E.screenrows - 1) {
            write(STDOUT_FILENO, "\r\n", 2);
        }
    }
}

void editorRefreshScreen() {
    //scroll edit feature added! from step3
    editorScroll();

    write(STDOUT_FILENO, "\x1b[?25l", 6); // Hide cursor (VT100)
    write(STDOUT_FILENO, "\x1b[H", 3);    // Move cursor to top-left (VT100)

    editorDrawRows();

    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", 
             (E.cy - E.rowoff) + 1, (E.cx - E.coloff) + 1); //커서를 올바른 위치에 이동시킨다. diff from step3) 이번엔 스크롤 considering
    write(STDOUT_FILENO, buf, strlen(buf));

    write(STDOUT_FILENO, "\x1b[?25h", 6); // 커서 이동 후, 커서를 다시 보이게 하기 (VT100)
}

/* Input handling */
int editorReadKey() {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) die("read");
    }

    if (c == '\x1b') {
        char seq[3];
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return ARROW_UP;
                case 'B': return ARROW_DOWN;
                case 'C': return ARROW_RIGHT;
                case 'D': return ARROW_LEFT;
            }
        }
        return '\x1b';
    }
    return c;
}

void editorMoveCursor(int key) {
    erow *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];

    switch (key) {
        case ARROW_LEFT:
            if (E.cx != 0) {
                E.cx--;
            } else if (E.cy > 0) {
                E.cy--;
                E.cx = E.row[E.cy].size;
            }
            break;
        case ARROW_RIGHT:
            if (row && E.cx < row->size) {
                E.cx++;
            } else if (row && E.cx == row->size) {
                E.cy++;
                E.cx = 0;
            }
            break;
        case ARROW_UP:
            if (E.cy != 0) E.cy--;
            break;
        case ARROW_DOWN:
            if (E.cy < E.numrows) E.cy++;
            break;
    }

    row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
    int rowlen = row ? row->size : 0;
    if (E.cx > rowlen) {
        E.cx = rowlen;
    }
}

void editorProcessKeypress() {
    int c = editorReadKey();

    switch (c) {
        case CTRL_KEY('q'):
            write(STDOUT_FILENO, "\x1b[2J", 4);
            write(STDOUT_FILENO, "\x1b[H", 3);
            exit(0);
            break;

        case CTRL_KEY('s'):
            editorSave();
            break;

        case BACKSPACE:
            editorDelChar();
            break;

        case ENTER:
            editorInsertNewline();
            break;

        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_LEFT:
        case ARROW_RIGHT:
            editorMoveCursor(c);
            break;

        default:
            if (!iscntrl(c)) {
                editorInsertChar(c);
            }
            break;
    }
}

/* File I/O */
// 모든 줄을 하나의 문자열로 변환 in order to file save
char *editorRowsToString(int *buflen) {
    //1. 하나의 char*로 저장할거니까, 전체 필요한 길이 부터 계산 
    int totlen = 0;
    int j;
    for (j = 0; j < E.numrows; j++)
        totlen += E.row[j].size + 1; // +1은 \n을 위한 공간
    *buflen = totlen;

    //2. 버퍼 할당 
    char *buf = malloc(totlen); //editor에 모든 줄을 하나의 줄, char*에 저장한다.
    char *p = buf;

    //3. 각 줄을 순차적으로 복사하고 \n추가 
    for (j = 0; j < E.numrows; j++) {
        memcpy(p, E.row[j].chars, E.row[j].size);
        p += E.row[j].size;
        *p = '\n'; //줄 구분을 '\n'으로 한다. 
        p++;
    }

    //추후, editorOpen()에서 \n을 기준으로 한줄이었던걸 linebreak해서 appendRow() 한다.
    return buf;
}

// open file 
void editorOpen(char *filename) {
    free(E.filename);
    E.filename = strdup(filename); // 파일 이름 저장

    FILE *fp = fopen(filename, "r");
    if (!fp) return;

    // getline()으로 파일 내용을 한 줄씩 읽어서 버퍼에 저장
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    //getline()이 자동으로 \n을 기준으로 줄을 분리
    while ((linelen = getline(&line, &linecap, fp)) != -1) { //getline() is from stdio.h, which gets total number of line of file
        // 줄 끝의 개행문자(\n or \r) 제거
        while (linelen > 0 && (line[linelen-1] == '\n' ||  
                              line[linelen-1] == '\r'))
            linelen--;
        //각 줄을 새로운 line append()
        editorAppendRow(line, linelen); //append to E.erow line by line
    }
    free(line);
    fclose(fp);
    E.dirty = 0; // 파일을 방금 읽었으므로 수정되지 않은 상태
}

void editorSave() {
    if (E.filename == NULL) return; //처음부터 filename을 입력했어야 했네. editorOpen(char* filename) 으로 연 상태여야 ctrl + s로 저장되는거네 

    // 모든 줄을 하나의 문자열로 변환
    int len;
    char *buf = editorRowsToString(&len);

    int fd = open(E.filename, O_RDWR | O_CREAT, 0644);
    if (fd != -1) {
        if (ftruncate(fd, len) != -1) {
            if (write(fd, buf, len) == len) {
                close(fd);
                free(buf);
                E.dirty = 0;
                return;
            }
        }
        close(fd);
    }
    free(buf);
}

/* Init */
//mostly same from step3, except for E.dirty and E.filename, dealing with file save & open
void initEditor() {
    E.cx = 0;
    E.cy = 0;
    E.rowoff = 0;
    E.coloff = 0;
    E.numrows = 0;
    E.row = NULL;
    E.dirty = 0;
    E.filename = NULL;

    if (getWindowSize(&E.screenrows, &E.screencols) == -1) die("getWindowSize");
    E.screenrows -= 1;  // Make room for status bar
}

/* Main */
int main(int argc, char *argv[]) {
    enableRawMode();
    initEditor();

    if (argc >= 2) {
        //if ./editor ${file_path_with_name}, open that file
        editorOpen(argv[1]);
    }

    while (1) {
        editorRefreshScreen();
        editorProcessKeypress();
    }

    return 0;
}
