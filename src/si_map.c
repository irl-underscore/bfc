#include "si_map.h"

#include <stdlib.h>
#include <string.h>

struct pair
{
    hash hash;
    void *data;
};

struct si_map
{
    size_t length;
    size_t capacity;
    size_t element_size;
    struct pair pairs[];
};

si_map *si_map_create(size_t initial_capacity, size_t value_size, size_t element_size)
{
    initial_capacity = (initial_capacity == 0) ? 1 : initial_capacity;
    value_size = (value_size == 0) ? 1 : value_size;
    element_size = (element_size == 0) ? 1 : element_size;
    si_map *map = malloc(sizeof(si_map) + sizeof(struct pair) * initial_capacity);
    if (!map) return NULL;

    map->length = 0;
    map->capacity = initial_capacity;
    return map;
}

static void si_map_realloc(si_map **map)
{
    if (!map) return;

    size_t new_size = (*map)->capacity * 2;
    si_map *temp = realloc(map, sizeof(si_map) + new_size * (*map)->element_size);
    if (!temp) return;

    (*map) = temp;
    (*map)->capacity = new_size;
}

void si_map_insert(si_map *map, hash key, void *data)
{
    if (!map | !data) return;

    if (map->length >= map->capacity)
    {
        si_map_realloc(&map);
    }

    struct pair input = {
        .hash = key
    };

    void *temp = malloc(map->element_size);
    if (!temp) return;

    memcpy(temp, data, map->element_size);
    input.data = temp;
    memcpy(map->pairs + map->length, &input, sizeof(struct pair));
    map->length++;
}

void *si_map_get(si_map *map, hash key)
{
    if (!map || key >= map->length) return NULL;

    for (size_t i = 0; i < map->length; i++)
    {
        if (map->pairs[i].hash == key)
        {
            return map->pairs[i].data;
        }
    }

    return NULL;
}

void si_map_desrtoy(si_map *map)
{
    if (!map) return;

    for (size_t i = 0; i < map->length; i++)
    {
        if (map->pairs[i].data) free(map->pairs[i].data);
    }

    free(map);
}
