#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for a single line of text
typedef struct textrow {
    char *chars;    // Actual text content
    int size;       // Size of the text
    int capacity;   // Allocated capacity
} textrow;

// Structure for the entire text buffer
typedef struct textbuffer {
    textrow *rows;  // Array of text rows
    int numrows;    // Number of rows
    int capacity;   // Allocated capacity for rows
} textbuffer;

// Initialize a new text buffer
textbuffer* createBuffer() {
    textbuffer *buf = malloc(sizeof(textbuffer));
    buf->rows = NULL;
    buf->numrows = 0;
    buf->capacity = 0;
    return buf;
}

// Append a new line to the buffer
void appendLine(textbuffer *buf, const char *line) {
    // Grow the rows array if needed
    if (buf->numrows >= buf->capacity) {
        int new_capacity = buf->capacity ? buf->capacity * 2 : 4;
        buf->rows = realloc(buf->rows, new_capacity * sizeof(textrow));
        buf->capacity = new_capacity;
    }
    
    // Initialize the new row
    int idx = buf->numrows;
    int len = strlen(line);
    buf->rows[idx].chars = malloc(len + 1);
    memcpy(buf->rows[idx].chars, line, len + 1);
    buf->rows[idx].size = len;
    buf->rows[idx].capacity = len + 1;
    buf->numrows++;
}

// Insert text at a specific position in a row
void insertChar(textrow *row, int at, char c) {
    if (at < 0 || at > row->size) return;
    
    // Grow the char array if needed
    if (row->size + 1 >= row->capacity) {
        int new_capacity = row->capacity * 2;
        row->chars = realloc(row->chars, new_capacity);
        row->capacity = new_capacity;
    }
    
    // Make room for the new character
    memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
    row->chars[at] = c;
    row->size++;
}

// Delete text at a specific position in a row
void deleteChar(textrow *row, int at) {
    if (at < 0 || at >= row->size) return;
    memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
    row->size--;
}

// Free the buffer and all its contents
void freeBuffer(textbuffer *buf) {
    for (int i = 0; i < buf->numrows; i++) {
        free(buf->rows[i].chars);
    }
    free(buf->rows);
    free(buf);
}

// Example usage
int main() {
    textbuffer *buf = createBuffer();
    
    // Add some lines
    appendLine(buf, "First line");
    appendLine(buf, "Second line");
    appendLine(buf, "Third line");
    
    // Insert and delete characters
    insertChar(&buf->rows[0], 5, '-');
    deleteChar(&buf->rows[1], 0);
    
    // Print the buffer contents
    for (int i = 0; i < buf->numrows; i++) {
        printf("%.*s\n", buf->rows[i].size, buf->rows[i].chars);
    }
    
    freeBuffer(buf);
    return 0;
}
