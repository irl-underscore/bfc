#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H
#include <stdint.h>

typedef struct dyn_array dyn_array;

dyn_array *dyn_array_create(size_t intial_size, size_t obj_size);
void dyn_array_destroy(dyn_array *arr);

#endif
