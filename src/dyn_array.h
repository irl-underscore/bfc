#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H
#include <stdint.h>

typedef struct dyn_array dyn_array;

dyn_array *dyn_array_create(size_t intial_size, size_t obj_size);
void dyn_array_insert_end(dyn_array *arr, void *obj);
void dyn_array_restrict_insert_end(dyn_array *__restrict__ arr, void *__restrict__ obj);
size_t dyn_array_get_size(dyn_array *arr);
void *dyn_array_get(dyn_array *arr, size_t index);
void dyn_array_destroy(dyn_array *arr);

#endif
