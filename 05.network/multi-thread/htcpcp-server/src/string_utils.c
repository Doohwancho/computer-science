#include "string_utils.h"

#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void string_concat(char **dest, const char *src)
{
    if (*dest)
    {
        size_t dest_length = strlen(*dest);
        char *temp = malloc(dest_length + 1);
        strncpy(temp, *dest, dest_length);
        temp[dest_length] = 0;

        size_t src_length = strlen(src);
        size_t new_length = dest_length + src_length;
        *dest = realloc(*dest, new_length + 1);
        strncpy(*dest, temp, dest_length);
        strncat(*dest, src, src_length);
        (*dest)[new_length] = 0;

        free(temp);
    }
    else
    {
        size_t length = strlen(src);
        *dest = malloc(length + 1);
        strncpy(*dest, src, length);
        (*dest)[length] = 0;
    }
}

char *string_format(const char *format, ...)
{
    va_list argv;
    va_start(argv, format);
    size_t length = vsnprintf(NULL, 0, format, argv);
    char *string = malloc(length + 1);
    vsprintf(string, format, argv);
    string[length] = 0;
    va_end(argv);
    return string;
}
