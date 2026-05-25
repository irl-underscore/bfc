#include "dyn_array.h"

#include "type.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

static void dyn_array_resize(dyn_array **arr)
{
    size_t new_capacity = (*arr)->capacity * 2;
    size_t total_bytes = sizeof(dyn_array) + (new_capacity * (*arr)->obj_size);
    dyn_array *temp = realloc(*arr, total_bytes);
    if (!temp) {
        return;
    }

    temp->capacity = new_capacity;
    (*arr) = temp;
}

void dyn_array_insert_end(dyn_array *arr, void *obj)
{
    if (!arr || !obj) return;

    if (arr->index >= arr->capacity)
    {
        uint8_t valide = 0;
        size_t offset = 0;
        if ((byte*)obj >= &arr->data[0] || (byte*)obj <= &arr->data[arr->capacity])
        {
            valide = 1;
            offset = ((byte*)obj - &arr->data[0]) * arr->obj_size;
        }

        dyn_array_resize(&arr);
        if (valide) obj = &arr->data[offset];
    }

    void *dest = (byte*)arr->data + (arr->index * arr->obj_size);
    memmove(dest, obj, arr->obj_size);
    arr->index++;
}

void dyn_array_restrict_insert_end(dyn_array *__restrict__ arr, void *__restrict__ obj)
{
    if (!arr || !obj) return;

    if (arr->index >= arr->capacity)
    {
        dyn_array *new_arr = arr;
        dyn_array_resize(&new_arr);
        arr = new_arr;
    }

    void *dest = (byte*)arr->data + (arr->index * arr->obj_size);
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
