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
    set_size(cur, cur->alligned_size);
}

size_t count_zone_of_type_empty(zone_t **zones, zonetype_t type) {
    size_t out = 0;
    zone_t *cur = *zones;
    while (cur)
    {
        if (cur->type == type) {
            if (cur->blocks->prev == NULL && cur->blocks->next == NULL && get_free(cur->blocks)) {
                out++;
            }
        }
        cur = cur->next;
    }
    return out;
}

void free_tiny_small_zone(zone_t **zones, zone_t *zone, block_t *block) {
    set_free(block, 1);
    set_size(block, block->alligned_size);
    if (block->next && get_free(block->next)) {
        join_blocks(block, block->next);
    } 
    if (block->prev && get_free(block->prev)) {
        block_t *temp = block->prev;
        join_blocks(block->prev, block);
        block = temp;
    }
    if (zone->type == LARGE) {
        free_zone(zones, zone);
    }
    else if (!block->prev && !block->next && count_zone_of_type_empty(zones, zone->type) > 1) {
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

void unsafe_free(zone_t **zones, void *ptr) {
    zone_t *zone;
    block_t *block;
    uintptr_t saved = (uintptr_t)ptr;
    if (!ptr) {
        return;
    }

    zone = get_zone(ptr);
    block = (block_t *)((intptr_t)ptr - allign_size(sizeof(block_t)));
    free_block(zones, zone, block);

    if (get_debug()->debug) {
        print_str("[Malloc] freed address: ");
        print_hexa(saved);
        print_str("\n");
    }
}