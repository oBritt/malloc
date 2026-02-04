#include "malloc.h"


static void handle_split(block_t *block, size_t size) {
    size_t old_block_size = block->alligned_size;
    size_t new_size_aligned = allign_size(size);
    size_t needed_from_next = new_size_aligned - old_block_size;
    block_t *next_block = block->next;

    if (next_block->alligned_size >= needed_from_next) {
        block_t *remaining_block = (block_t *)((char *)next_block + needed_from_next + allign_size(sizeof(block_t)));
        
        size_t remain_s = next_block->alligned_size - needed_from_next - allign_size(sizeof(block_t));
        remaining_block->alligned_size = remain_s;
        set_size(remaining_block, remain_s);
        set_free(remaining_block, 1);

        remaining_block->next = next_block->next;
        remaining_block->prev = block;
        if (next_block->next) next_block->next->prev = remaining_block;

        block->next = remaining_block;
        block->alligned_size = new_size_aligned;
        set_size(block, size);
    } else {
        block->next = block->next->next;
        if (block->next) {
            block->next->prev = block;
        }
        set_size(block, size);
        block->alligned_size = old_block_size + allign_size(sizeof(block_t)) + next_block->alligned_size;
    }
}

static void print_debug(void *ptr, size_t size) {
    if (get_debug()->debug) {
        print_str("[Malloc] realocated pointer ");
        print_hexa((uintptr_t)ptr);
        print_str(" to size ");
        print_decimal(size);
        print_str("\n");
    }
}

static void fill_zeros(void *ptr, size_t size, size_t original) {
    if (get_debug()->zero_alloc) {
        ft_memset((void *)((uintptr_t)ptr + original), size - original, 0);
    }
}

static void fill(void *dst, void *src, size_t size) {
    for (size_t i = 0; i < size; i++) {
        ((char *)dst)[i] = ((char *)src)[i];
    }
}

void *unsafe_realloc(zone_t **zones, void *ptr, size_t size) {
    block_t *block;
    zone_t *zone;
    void *new_ptr;

    if (!ptr) {
        return NULL;
    }
    zone = get_zone(ptr);
    block = (block_t *)((intptr_t)ptr - allign_size(sizeof(block_t)));
    size_t original_size = get_size(block);
    if (size <= block->alligned_size) {
        set_size(block, size);
        print_debug(ptr, size);
        fill_zeros(ptr, size, original_size);
        return ptr;
    }
    if (zone->type != LARGE) {
        if (block->next && get_free(block->next)
            && block->next->alligned_size + block->alligned_size + allign_size(sizeof(block_t)) >= size) {
            handle_split(block, size);
            print_debug(ptr, size);
            fill_zeros(ptr, size, original_size);
            return ptr;
        }
    }
    new_ptr = unsafe_malloc(zones, size);
    fill(new_ptr, ptr, original_size);
    unsafe_free(zones, ptr);
    return new_ptr;
}