#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


/*
---
What 


1. basic terminal setup
2. takes input char -> print ASCII code  


---
Takeaway


1. basic termios.h 

*/

struct termios orig_termios;

void die(const char *s) {
    perror(s);
    exit(1);
}

//restore original terminal setting
void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

// terminal raw mode 
void enableRawMode() {
    //step1) get the current terminal attributes -> orig_termios에 담음
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");

    //register disable mode to be called automatically
    atexit(disableRawMode);
    
    //make copy of original setting
    struct termios raw = orig_termios; //step2) orig_termios 변수에서 특정 flag를 바꿀 예정임 

    // Modify input flags (~: bitwise NOT)
    raw.c_iflag &= ~(
        ICRNL |    // Disable CR to NL translation
        IXON       // Disable software flow control (Ctrl-S, Ctrl-Q)
    );
    
    // Modify output flags
    raw.c_oflag &= ~(
        OPOST      // Disable all output processing
    );
    
    // Modify local flags
    raw.c_lflag &= ~(
        ECHO |     // Disable echoing of input characters
        ICANON |   // Disable canonical mode (line-by-line input)
        IEXTEN |   // Disable extended input processing
        ISIG       // Disable signal generation (Ctrl-C, Ctrl-Z)
    );
    
    // step3) apply modified settings(raws)
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

int main() {
    enableRawMode();
    
    while (1) {
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == -1) die("read"); //Q. read()가 &c씩 1글자(char)씩 읽는건가?
        if (c == 'q') break;
        
        // Print ASCII value of characters
        printf("Input char: %d ('%c')\r\n", c, c);
    }
    
    return 0;
}
