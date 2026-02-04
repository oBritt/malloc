
#include "malloc.h"


static block_t *create_default_block(zone_t *zone) {
    block_t *block = (block_t *)(((intptr_t)zone) + allign_size(sizeof(zone_t)));
    ft_memset(block, sizeof(block_t), 0);
    set_free(block, 1);
    block->next = NULL;
    block->prev = NULL;
    size_t size = zone->size - allign_size(sizeof(zone_t)) - allign_size(sizeof(block_t));
    set_size(block, size);
    block->alligned_size = size;
    return block;
}

static zone_t *create_tiny_zone(size_t size) {
    (void) size;
    size_t zone_size = get_size_of_tiny();
    zone_t *zone = allocate_memory(zone_size);

    ft_memset(zone, sizeof(zone_t), 0);
    zone->type = TINY;
    zone->next = NULL;
    zone->size = zone_size;
    zone->blocks = create_default_block(zone);
    zone->prev = NULL; 
    return zone;
}

static zone_t *create_small_zone(size_t size) {
    (void) size;
    size_t zone_size = get_size_of_small();
    zone_t *zone = allocate_memory(zone_size);
    
    ft_memset(zone, sizeof(zone_t), 0);
    zone->type = SMALL;
    zone->next = NULL;
    zone->size = zone_size;
    zone->blocks = create_default_block(zone);
    zone->prev = NULL; 
    return zone;
}

static zone_t *create_large_zone(size_t size) {

    size_t total = size + allign_size(sizeof(zone_t)) + allign_size(sizeof(block_t));
    total = (total / getpagesize() + 1) * getpagesize();
    zone_t* zone = allocate_memory(total);

    ft_memset(zone, sizeof(zone_t), 0);
    zone->type = LARGE;
    zone->size = total;
    zone->blocks = create_default_block(zone);
    zone->next = NULL;
    zone->prev = NULL; 
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