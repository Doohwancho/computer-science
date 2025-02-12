#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

struct termios orig_termios; // Store original terminal settings

// Restore terminal to original settings
void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

// Set up terminal raw mode
void enableRawMode() {
    // Get the current terminal settings
    tcgetattr(STDIN_FILENO, &orig_termios);
    // Register cleanup function at exit
    atexit(disableRawMode);
    
    // Make a copy to modify
    struct termios raw = orig_termios;
    
    // Modify the copy:
    // Input flags - disable break signal, CR to NL conversion,
    // parity checking, strip 8th bit, and flow control
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    
    // Output flags - disable post-processing
    raw.c_oflag &= ~(OPOST);
    
    // Control flags - set 8 bit chars
    raw.c_cflag |= (CS8);
    
    // Local flags - disable echo, canonical mode,
    // meta-characters, and signal chars
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    
    // Control chars - set return condition: min number of bytes and timer
    raw.c_cc[VMIN] = 0;  // Return each byte, or zero for timeout
    raw.c_cc[VTIME] = 1; // 100ms timeout
    
    // Apply the modified settings
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
    enableRawMode();
    
    // Read input character by character
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        // Print ASCII value of each character
        printf("Read: %d ('%c')\r\n", c, c);
    }
    
    return 0;
}
