#include "malloc.h"


void print_block(block_t* block, size_t *total, int detail) {
    intptr_t ptr;
    size_t start;
    size_t end;

    ptr = (intptr_t)block;
    start = ptr + allign_size(sizeof(block_t));
    end = start + block->size;

    if (!detail && block->free) {
        return;
    }
    if (!block->free) {
        *total += block->size;
    }
    print_hexa(start);
    print_str(" - ");
    print_hexa(end);
    print_str(" : ");
    print_decimal(block->size);

    if (detail) {
        if (block->free == 1) {
            print_str(" (not used)\n");
        }  else {
            print_str(" (used) ");
            print_decimal(block->alligned_size);
            print_str(" (alligned)\n");
        }
    }
}


void print_zone(zone_t* zone, size_t *total, int detail) {
    block_t *cur;
    
    cur = zone->blocks;
    if (zone->type == 0) {
        print_str("TINY: ");
    } else if (zone->type == 1) {
        print_str("SMALL: ");
    } else {
        print_str("LARGE: ");
    }
    size_t start = (size_t)((uintptr_t)zone);
    print_hexa(start);
    if (detail) {
        size_t end = start + zone->size;
        print_str(" - ");
        print_hexa(end);
    }
    print_str("\n");    
    while (cur)
    {
        print_block(cur, total, detail);
        cur = cur->next;
    }
    
}

void print_allocated(zone_t* zones, int detail) {
    zone_t* cur = zones;
    size_t total = 0;

    if (!zones) {
        print_str("nothing is allocated yet\n");
        return;
    }

    while (cur)
    {
        print_zone(cur, &total, detail);
        cur = cur->next;
    }
    print_str("Total : ");
    print_decimal(total);
    print_str(" bytes\n");
}