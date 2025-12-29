
#include "malloc.h"

size_t allign_size(size_t size) {
    return (size + ALLIGNMENT - 1) & (~(ALLIGNMENT - 1));
}

size_t get_size_of_tiny() {
    size_t page_size = getpagesize();
    size_t block_size = TINY_MAX + allign_size(sizeof(block_t));
    size_t total_size = block_size * MINIMUM_AMOUNT_OF_BLOCKS + allign_size(sizeof(zone_t));
    return ((total_size / page_size) + 1) * page_size * 2;
}

size_t get_size_of_small() {
    size_t page_size = getpagesize();
    size_t block_size = SMALL_MAX + allign_size(sizeof(block_t));
    size_t total_size = block_size * MINIMUM_AMOUNT_OF_BLOCKS + allign_size(sizeof(zone_t));
    return ((total_size / page_size) + 1) * page_size * 2;
}


zonetype_t get_zone_base_on_size(size_t size) {
    size_t alligned = allign_size(size);

    if (alligned > SMALL_MAX) {
        return LARGE;
    } else if (alligned > TINY_MAX) {
        return SMALL;
    } else {
        return TINY;
    }
}