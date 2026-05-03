#include "dyn_array.h"

#include "math.h"

#include <stdlib.h>
#include <string.h>

struct dyn_array
{
    size_t obj_size;
    size_t capacity;
    size_t index;
    byte data[];
};

dyn_array *dyn_array_create(size_t initial_size, size_t obj_size)
{
    size_t _initial_size = (initial_size == 0) ? 1 : initial_size;
    size_t _obj_size = (obj_size == 0) ? 1 : obj_size;
    dyn_array *arr = malloc(sizeof(dyn_array) + _initial_size * _obj_size);
    if (!arr) return NULL;

    arr->capacity = _initial_size;
    arr->obj_size = _obj_size;
    arr->index = 0;
    return arr;
}

dyn_array *dyn_array_resize(dyn_array *arr)
{
    size_t new_capacity = arr->capacity * 2;
    size_t total_bytes = sizeof(dyn_array) + (new_capacity * arr->obj_size);

    dyn_array *temp = realloc(arr, total_bytes);
    if (!temp) return arr;

    temp->capacity = new_capacity;
    return temp;
}

void dyn_array_emplace_back(dyn_array *arr, void *obj)
{
    if (!arr || !obj) return;

    if (arr->index >= arr->capacity) arr = dyn_array_resize(arr);


    void *dest = (unsigned char*)arr->data + (arr->index * arr->obj_size);
    memcpy(dest, obj, arr->obj_size);

    arr->index++;
}

size_t dyn_array_get_size(dyn_array *arr)
{
    if (arr) return arr->index;

    return arr->index;
}

void *dyn_array_get(dyn_array *arr, size_t index)
{
    if (!arr || index >= arr->index) return NULL;

    return (unsigned char*)arr->data + (index * arr->obj_size);
}

void dyn_array_destroy(dyn_array *arr)
{
    if (arr) free(arr);
}
