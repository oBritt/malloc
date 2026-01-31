
#include "malloc.h"


zone_t *g_zones = NULL;


void *malloc(size_t size) {
    zone_t *zone = NULL;
    block_t *block = NULL;

    zonetype_t type = get_zone_base_on_size(size);
    
    zone = find_free_zone(g_zones, size, type);
    if (!zone) {
        zone = create_zone(size, type);
        if (!zone) {
            return NULL;
        }
        insert_zone(&g_zones, zone);
    }

    block = create_block(zone, size);
    
    return (block_t *)((intptr_t)block + allign_size(sizeof(block_t))); 
}

void free(void *ptr) {
    zone_t *zone;
    block_t *block;

    zone = get_zone(ptr);
    block = (block_t *)((intptr_t)ptr - allign_size(sizeof(block_t)));
    free_block(&g_zones, zone, block);
}

void show_alloc_mem() {
    print_allocated(g_zones, false);
}

void show_alloc_mem_ex() {
    print_allocated(g_zones, true);
}