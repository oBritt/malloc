#include "malloc.h"

zone_t *get_zone(void *ptr) {
    block_t *block;

    block = (block_t *)((intptr_t)ptr - allign_size(sizeof(block_t)));
    while (block->prev) {
        block = block->prev;
    }
    return (zone_t *)((intptr_t)block - allign_size(sizeof(zone_t)));
}

void free_zone(zone_t **zones, zone_t *zone) {
    zone_t *prev;
    zone_t *next;

    prev = zone->prev;
    next = zone->next;
    if (prev) {
        prev->next = next;
    }
    if (next) {
        next->prev = prev;
    }
    if (*zones == zone) {
        *zones = zone->next;
    }
    deallocate_memory(zone, zone->size);
}

void join_blocks(block_t *cur, block_t *next) {
    cur->next = next->next;
    if (next->next) {
        next->next->prev = cur;
    }
    cur->alligned_size += next->alligned_size + allign_size(sizeof(block_t));
    cur->size = cur->alligned_size;
}

void free_tiny_small_zone(zone_t **zones, zone_t *zone, block_t *block) {
    block->free = 1;
    block->size = block->alligned_size;
    if (block->next && block->next->free) {
        join_blocks(block, block->next);
    } 
    if (block->prev && block->prev->free) {
        join_blocks(block->prev, block);
        block = block->prev;
    }
    if (!block->prev && !block->next) {
        free_zone(zones, zone);
    }
}


void free_large_zone(zone_t **zones, zone_t *zone, block_t *block) {
    (void) block;

    free_zone(zones, zone);
}

void free_block(zone_t **zones, zone_t *zone, block_t *block) {
    static int first = 1;
    static void (*func_ptr[3])(zone_t **, zone_t *, block_t *);

    if (first) {
        first = 0;
        func_ptr[TINY] = free_tiny_small_zone;
        func_ptr[SMALL] = free_tiny_small_zone;
        func_ptr[LARGE] = free_large_zone;
    }
    func_ptr[zone->type](zones, zone, block);
}
