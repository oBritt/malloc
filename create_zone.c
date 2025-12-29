
#include "malloc.h"


static block_t *create_default_block(zone_t *zone) {
    block_t *block = (block_t *)(zone + 1);
    block->free = 1;
    block->next = NULL;
    block->prev = NULL;
    block->size = zone->size - sizeof(zone_t) - sizeof(block_t);
    
    return block;
}

static zone_t *create_tiny_zone(size_t size) {
    (void) size;
    size_t zone_size = get_size_of_tiny();
    zone_t *zone = allocate_memory(zone_size);

    zone->type = TINY;
    zone->next = NULL;
    zone->blocks = create_default_block(zone);
    zone->size = zone_size;
    return zone;
}

static zone_t *create_small_zone(size_t size) {
    (void) size;
    size_t zone_size = get_size_of_small();
    zone_t *zone = allocate_memory(zone_size);

    zone->type = SMALL;
    zone->next = NULL;
    zone->blocks = create_default_block(zone);
    zone->size = zone_size;
    return zone;
}

static zone_t *create_large_zone(size_t size) {
    zone_t* zone = allocate_memory(size + sizeof(zone_t));

    zone->type = LARGE;
    zone->size = size + sizeof(zone_t);
    zone->blocks = create_default_block(zone);
    zone->next = NULL;
    return zone;
}

zone_t *create_zone(size_t size, zonetype_t type) {
    static int first = 1;
    static zone_t *(*func_ptr[3])(size_t);

    if (first) {
        first = 0;
        func_ptr[TINY] = create_tiny_zone;
        func_ptr[SMALL] = create_small_zone;
        func_ptr[LARGE] = create_large_zone;
    }
    return func_ptr[type](size);
}