#include "malloc.h"


static block_t *split(block_t *block, size_t size) {
    
    size_t remaining_size = block->alligned_size - allign_size(sizeof(block_t)) - allign_size(size);

    block_t *next = block->next;

    set_size(block, size);
    set_free(block, 0);
    block->alligned_size = allign_size(size);

    block_t *new_block = (block_t *)((intptr_t)block + allign_size(sizeof(block_t)) + allign_size(size)); 
    ft_memset(new_block, sizeof(block_t), 0);

    new_block->prev = block;
    new_block->next = next;
    if (next) {
        next->prev = new_block;
    }
    block->next = new_block;
    set_size(new_block, remaining_size);
    set_free(new_block, 1);
    new_block->alligned_size = remaining_size;
    return block;
}


static block_t *split_block(block_t *block, size_t size) {
    if (allign_size(size) + allign_size(sizeof(block_t)) >= block->alligned_size) {
        set_size(block, size);
        set_free(block, 0);
        return block;
    }
    return split(block, size);
}


static block_t *create_block_tiny_small(zone_t *zone, size_t size) {
    block_t *cur = zone->blocks;

    while (cur) {
        if (get_free(cur) && cur->alligned_size >= allign_size(size)) {
            return split_block(cur, size);
        }
        cur = cur->next;
    }
    
    // should never run
    return NULL;
}


static block_t *create_block_large(zone_t *zone, size_t size) {
    block_t *out;

    (void)size;
    out = zone->blocks;
    set_free(out, 0);
    return out;
}

block_t *create_block(zone_t *zone, size_t size) {

    static int first = 1;
    static block_t *(*func_ptr[3])(zone_t *, size_t);

    if (first) {
        first = 0;
        func_ptr[TINY] = create_block_tiny_small;
        func_ptr[SMALL] = create_block_tiny_small;
        func_ptr[LARGE] = create_block_large;
    }
    return func_ptr[zone->type](zone, size);
   
}