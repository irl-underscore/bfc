#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H
#include <stdint.h>

typedef struct dyn_array dyn_array;

dyn_array *dyn_array_create(size_t intial_size, size_t obj_size);
static void dyn_array_resize(dyn_array *arr) __attribute__((always_inline));
void dyn_array_emplace_back(dyn_array *arr, void *obj);
void *dyn_array_get(dyn_array *arr, size_t index);
void dyn_array_destroy(dyn_array *arr);

#endif
