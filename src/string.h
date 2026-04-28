/*
 * So the string struct is build like this:
 * if initial_size = 3
 * the structure would look like this:
 * ['', '', '', '\0']
 * wich means that the acutall size of the string is
 * actually one bigger than the struct says.
 * Note: Yea I used actuall/y 2 times :)
 * So when I acess str->data[capacity] it returns the last
 * char and not '\0'
 */

#ifndef STRING_H
#define STRING_H
#include <stdint.h>

typedef struct string string;

string *string_create(size_t initial_size);
string *string_copy(char *data);
static void string_realloc(string *str, size_t target_size) __attribute__((always_inline));
void string_append_string(string *str, char *data);
void stirng_append_char(string *str, char c);
char *string_get_raw(string *str);
void string_destroy(string *str);

#endif
