#ifndef SI_MAP_H
#define SI_MAP_H
#include <stdint.h>

typedef unsigned long long hash;

typedef struct si_map si_map;

si_map *si_map_create(size_t initial_capacity, size_t value_size, size_t element_size);
void si_map_insert(si_map *map, hash key, void *data);
void *si_map_get(si_map *map, hash key);
void si_map_desrtoy(si_map *map);

#endif
