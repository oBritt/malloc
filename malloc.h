
#ifndef MALLOC_H
#define MALLOC_H



#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>

typedef enum {
    TINY,
    SMALL,
    LARGE
} zonetype_t;

typedef struct s_block
{
    size_t size;
    int free;
    struct s_block *next;
    struct s_block *prev;
}   block_t;


typedef struct s_zone {
    size_t size;
    zonetype_t type;
    struct s_zone *next;
    block_t *blocks;
}   zone_t;

#define MINIMUM_AMOUNT_OF_BLOCKS 100
#define TINY_MAX 256 // 2 ^ 8 
#define SMALL_MAX 65536 // 2 ^ 16

#define ALLIGNMENT 16   // must be power of 2


size_t allign_size(size_t size);
size_t get_size_of_tiny();
size_t get_size_of_small();
zonetype_t get_zone_base_on_size(size_t size);
void *allocate_memory(size_t size);
zone_t *create_zone(size_t size, zonetype_t type);
zone_t *find_free_zone(zone_t *zones, size_t size, zonetype_t type);
void insert_zone(zone_t **zones, zone_t* zone);

#endif