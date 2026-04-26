#include "dyn_array.h"

#include "math.h"

#include <stdlib.h>

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
