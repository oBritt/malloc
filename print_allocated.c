#include "malloc.h"


void print_block(block_t* block, size_t *total) {
    intptr_t ptr;
    size_t start;
    size_t end;

    ptr = (intptr_t)block;
    start = ptr + allign_size(sizeof(block_t));
    end = start + block->size;

    print_hexa(start);
    write(1, " - ", 3);
    print_hexa(end);
    write(1, " : ", 3);
    print_decimal(block->size);
    if (block->free == 1) {
        write(1, " (not used)\n", 12);
    }  else {
        write(1, " (used)\n", 8);
        *total += block->size;
    }
}


void print_zone(zone_t* zone, size_t *total) {
    block_t *cur;
    
    cur = zone->blocks;
    if (zone->type == 0) {
        write(1, "TINY: ", 6);
    } else if (zone->type == 1) {
        write(1, "SMALL: ", 7);
    } else {
        write(1, "LARGE: ", 7);
    }
    print_hexa((size_t)((uintptr_t)zone));
    write(1, "\n", 1);    
    while (cur)
    {
        print_block(cur, total);
        cur = cur->next;
    }
    
}

void print_allocated(zone_t* zones) {
    zone_t* cur = zones;
    size_t total = 0;

    if (!zones) {
        write(1, "nothing is allocated yet\n", 25);
        return;
    }

    while (cur)
    {
        print_zone(cur, &total);
        cur = cur->next;
    }
    write(1, "Total : ", 8);
    print_decimal(total);
    write(1, " bytes\n", 7);
}