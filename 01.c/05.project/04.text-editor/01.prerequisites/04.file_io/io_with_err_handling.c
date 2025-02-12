#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define CHUNK_SIZE 4096

// Structure to hold file content
typedef struct {
    char *data;
    size_t size;
} FileBuffer;

// Read file contents efficiently
FileBuffer readFile(const char *filename) {
    FileBuffer buffer = {NULL, 0};
    FILE *fp = fopen(filename, "r");
    
    if (!fp) {
        fprintf(stderr, "Error opening file '%s': %s\n", 
                filename, strerror(errno));
        return buffer;
    }
    
    char chunk[CHUNK_SIZE];
    size_t bytesRead;
    
    // Read file in chunks
    while ((bytesRead = fread(chunk, 1, CHUNK_SIZE, fp)) > 0) {
        // Expand buffer
        char *new_data = realloc(buffer.data, buffer.size + bytesRead + 1);
        if (!new_data) {
            fprintf(stderr, "Failed to allocate memory\n");
            free(buffer.data);
            buffer.data = NULL;
            buffer.size = 0;
            fclose(fp);
            return buffer;
        }
        
        buffer.data = new_data;
        // Copy new chunk to buffer
        memcpy(buffer.data + buffer.size, chunk, bytesRead);
        buffer.size += bytesRead;
    }
    
    // Null terminate the buffer
    if (buffer.data) {
        buffer.data[buffer.size] = '\0';
    }
    
    fclose(fp);
    return buffer;
}

// Write buffer to file safely
int writeFile(const char *filename, const char *data, size_t size) {
    // Open file with write permissions
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        fprintf(stderr, "Error opening file for writing: %s\n", 
                strerror(errno));
        return -1;
    }
    
    // Write data in chunks
    size_t remaining = size;
    const char *ptr = data;
    
    while (remaining > 0) {
        ssize_t written = write(fd, ptr, remaining);
        if (written == -1) {
            if (errno == EINTR) {
                // Interrupted by signal, retry
                continue;
            }
            fprintf(stderr, "Error writing to file: %s\n", 
                    strerror(errno));
            close(fd);
            return -1;
        }
        remaining -= written;
        ptr += written;
    }
    
    // Ensure data is written to disk
    if (fsync(fd) == -1) {
        fprintf(stderr, "Error flushing file to disk: %s\n", 
                strerror(errno));
        close(fd);
        return -1;
    }
    
    close(fd);
    return 0;
}

int main() {
    // Example usage
    const char *filename = "test.txt";
    const char *test_data = "Hello, World!\nThis is a test.\n";
    
    // Write test data
    if (writeFile(filename, test_data, strlen(test_data)) == -1) {
        fprintf(stderr, "Failed to write file\n");
        return 1;
    }
    
    // Read it back
    FileBuffer buffer = readFile(filename);
    if (!buffer.data) {
        fprintf(stderr, "Failed to read file\n");
        return 1;
    }
    
    printf("Read from file:\n%s", buffer.data);
    
    free(buffer.data);
    return 0;
}
