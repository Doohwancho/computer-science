#ifndef MY_STRING_H
#define MY_STRING_H

#include <stddef.h>  // for size_t


#define END_STRING (char)'\0'

typedef struct ss {
    size_t len;  // 문자열의 길이 (널 종료 문자 포함) (read-only)
    size_t free; // 추가로 사용 가능한 메모리 공간. 미리 할당된 공간은 문자열 조작 시 재할당을 줄이기 위해 사용된다. (read-only)
    char *buf;   // 문자열 데이터를 가리키는 포인터. (can be modified)
} *ss; //Q. 왜 *가 붙음?

/**************************************
 * Basic String Operations
 **************************************/

// Length functions
size_t my_strlen(const char* str);
// size_t my_strnlen(const char* str, size_t maxlen);

// Copy functions
char* my_strcpy(char* dest, const char* src);
// char* my_strncpy(char* dest, const char* src, size_t n);
// size_t my_strlcpy(char* dest, const char* src, size_t size);

// Concatenation functions
// char* my_strcat(char* dest, const char* src);
// char* my_strncat(char* dest, const char* src, size_t n);
// size_t my_strlcat(char* dest, const char* src, size_t size);

// Comparison functions
// int my_strcmp(const char* s1, const char* s2);
// int my_strncmp(const char* s1, const char* s2, size_t n);

// Search functions
// char* my_strchr(const char* str, int c);
// char* my_strrchr(const char* str, int c);
// char* my_strstr(const char* haystack, const char* needle);
// char* my_strrstr(const char* haystack, const char* needle);

/**************************************
 * Extended String Operations
 **************************************/

// Duplication
// char* my_strdup(const char* str);
// char* my_strndup(const char* str, size_t n);

// Case conversion
// char* my_strupper(char* str);
// char* my_strlower(char* str);

// Trim functions
// char* my_strtrim(char* str);
// char* my_strtrim_left(char* str);
// char* my_strtrim_right(char* str);

// Tokenization
// char* my_strtok(char* str, const char* delim);
// char* my_strtok_r(char* str, const char* delim, char** saveptr);

// Span functions
// size_t my_strspn(const char* str, const char* accept);
// size_t my_strcspn(const char* str, const char* reject);

// Memory functions
void* my_memcpy(void* dest, const void* src, size_t n);
void* my_memmove(void* dest, const void* src, size_t n);
void* my_memset(void* str, int c, size_t n);
// int my_memcmp(const void* s1, const void* s2, size_t n);
// void* my_memchr(const void* str, int c, size_t n);

/**************************************
 * String Utility Functions
 **************************************/

// Conversion functions
// long my_strtol(const char* str, char** endptr, int base);
// unsigned long my_strtoul(const char* str, char** endptr, int base);
// double my_strtod(const char* str, char** endptr);

// String formatting
// int my_sprintf(char* str, const char* format, ...);
// int my_snprintf(char* str, size_t size, const char* format, ...);

// String splitting
// char** my_strsplit(const char* str, const char* delim, size_t* count);
// void my_strsplit_free(char** strv, size_t count);

// String joining
// char* my_strjoin(const char** strv, size_t count, const char* delim);

// String checking
// int my_isspace(int c);
// int my_isdigit(int c);
// int my_isalpha(int c);
// int my_isalnum(int c);
// int my_isupper(int c);
// int my_islower(int c);

// Error handling
// extern int my_string_errno;
// const char* my_string_error(int errnum);

#endif // MY_STRING_H
