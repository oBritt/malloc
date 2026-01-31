#include "malloc.h"


static void split_next_in_two(block_t *block, size_t size) {
    block_t *next_next = block->next->next;
    size_t memory_available = block->alligned_size + block->next->alligned_size;

    block->size = size;
    block->alligned_size = allign_size(size);
    block_t *next = (block_t *)((intptr_t)block + allign_size(sizeof(block_t)) + block->alligned_size);
    next->free = 1;
    next->alligned_size = memory_available - block->alligned_size;
    next->size = next->alligned_size;
    next_next->prev = next;
    next->next = next_next;
    block->next = next;
}

static void handle_split(block_t *block, size_t size, size_t min_space) {
    size_t memory_available = block->alligned_size + block->next->alligned_size + allign_size(sizeof(block_t));
    if (memory_available > allign_size(size) + allign_size(sizeof(block_t)) + min_space) {
        split_next_in_two(block->next, size);
    } else {
        block->next = block->next->next;
        block->next->prev = block;
        block->size = size;
        block->alligned_size = memory_available;
    }
}

void realloc_take_next_block(zone_t *zone, block_t *block, size_t size) {
    if (zone->type == SMALL) {
        handle_split(block, size, TINY_MAX);
    } else {
        handle_split(block, size, 0);
    }
}