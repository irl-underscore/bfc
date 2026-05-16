/*
 * So the string struct is build like this:
 * if initial_size = 3
 * the structure would look like this:
 * ['\0', '', '', '']
 * wich means that the acutal size of the string is
 * one bigger than the struct says.
 * So when I acess str->data[capacity] it returns NULL
 * and str->data[length] always returns '\0'
 */

#ifndef STRING_H
#define STRING_H
#include <stdint.h>

typedef struct string string;

string *string_create(size_t initial_size);
string *string_copy(const char *data);
void string_append_string(string *str, const char *data);
void string_append_format(string *str, const char *format, ...);
void stirng_append_char(string *str, char c);
char *string_get_raw(string *str);
void string_destroy(string *str);

#endif
