#include "string.h"

#include <stdlib.h>
#include <string.h>

struct string {
    size_t length;
    size_t capacity;
    char *data;
};

string *stirng_create(size_t initial_size)
{
    initial_size = (initial_size == 0) ? 1 : initial_size;
    string *str = malloc(sizeof(string));
    if (!str) return NULL;

    str->data = malloc(initial_size + 1);
    if (!str->data)
    {
        free(str);
        return NULL;
    }

    str->data[initial_size + 1] = '\0';
    str->capacity = initial_size;
    str->length = 0;
    return str;
}

string *string_copy(char *data)
{
    if (!data) return NULL;

    string *str = malloc(sizeof(string));
    if (!str) return NULL;

    str->data = data;
    str->length = strlen(data);
    str->capacity = str->length;
    return str;
}

static void string_realloc(string *str, size_t target_size)
{
    size_t new_capacity = str->capacity * 2;
    if (new_capacity < target_size) new_capacity = target_size;

    char *temp = realloc(str->data, new_capacity);
    if (!temp) return;

    str->data = temp;
    str->capacity = new_capacity;
}

void stirng_append_string(string *str, char *data)
{
    if (!str || !str->data || !data) return;

    size_t data_len = strlen(data);
    if (str->length + data_len > str->capacity)
    {
        string_realloc(str, str->length + data_len);
    }

    memcpy(str->data + (str->length + 1), data, data_len);
    str->length += data_len;
    str->data[str->length + 1] = '\0';
}

char *string_get_raw(string *str)
{
    if (!str || !str->data) return NULL;

    return str->data;
}

void string_destroy(string *str)
{
    if (!str) return;

    if (str->data) free(str->data);

    free(str);
}
