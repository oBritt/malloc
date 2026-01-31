
#include "malloc.h"

static void print_malloc(void *out, size_t size) {
    if (get_debug()->debug) {
        print_str("[Malloc] maloc allocated ");
        print_decimal(size);
        print_str(" bytes at address ");
        print_hexa((uintptr_t)out);
        print_str("\n");
    }
}

static int should_fail() {
    if (get_debug()->random_fail) {
        if (rand() % 10 == 0) {
            if (get_debug()->debug) {
                print_str("[Malloc] malloc failed\n");
            }
            return 1;
        }
    }
    return 0;
}

void *unsafe_malloc(zone_t **zones, size_t size) {
    if (should_fail()) {
        return NULL;
    }
    
    zone_t *zone = NULL;
    block_t *block = NULL;
    zonetype_t type = get_zone_base_on_size(size);
    
    zone = find_free_zone(*zones, size, type);
    if (!zone) {
        zone = create_zone(size, type);
        if (!zone) {
            if (get_debug()->debug) {
                print_str("[Malloc] malloc failed\n");
            }
            return NULL;
        }
        insert_zone(zones, zone);
    }

    block = create_block(zone, size);
    
    void *out = (block_t *)((intptr_t)block + allign_size(sizeof(block_t))); 
    print_malloc(out, size);

    if (get_debug()->zero_alloc) {
        ft_memset(out, size, 0);
    }
    return out;
}