
#include "malloc.h"

void set_free(block_t* block, int value) {
    size_t shift = sizeof(size_t) * 4 - 1;

    if (value) {
        block->size = block->size | ((size_t)1 << shift);
    } else {
        block->size = block->size & (~((size_t)1 << shift));
    }
}

int get_free(block_t* block) {
    size_t shift = sizeof(size_t) * 4 - 1;
    return block->size >> shift;
}


size_t get_size(block_t* block) {
    size_t shift = sizeof(size_t) * 4 - 1;
    return block->size & (~((size_t)1 << shift));
}

void set_size(block_t* block, size_t value) {
    size_t shift = sizeof(size_t) * 4 - 1;
    block->size = (block->size & ((size_t)1 << shift)) | (value & (~((size_t)1 << shift)));
}