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

static void dyn_array_resize(dyn_array *arr)
{
    size_t new_size = sizeof(dyn_array) + arr->capacity * 1.5;
    new_size = ALIGN_UP(new_size, arr->obj_size);
    void *temp_arr = realloc(arr, new_size);
    if (!temp_arr) return;

    arr = temp_arr;
    arr->capacity = new_size;
}

void dyn_array_emplace_back(dyn_array *arr, void *obj)
{
    if (!arr || !obj) return;

    if (arr->index + 1 >= arr->capacity)
    {
        dyn_array_resize(arr);
    }

    memmove(arr->data + arr->index, obj, arr->obj_size);
}

void dyn_array_destroy(dyn_array *arr)
{
    if (arr) free(arr);
}
