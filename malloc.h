
#ifndef MALLOC_H
#define MALLOC_H



#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <inttypes.h>
#include <limits.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h> 

typedef enum {
    TINY,
    SMALL,
    LARGE
} zonetype_t;

typedef struct s_block
{
    size_t size;
    size_t alligned_size;
    uint8_t free;
    struct s_block *next;
    struct s_block *prev;
}   block_t;


typedef struct s_zone {
    size_t size;
    zonetype_t type;
    struct s_zone *next;
    struct s_zone *prev;
    block_t *blocks;
}   zone_t;

typedef struct s_debug_config {
    int debug;
    int random_fail;
    int zero_alloc;
} debug_config_t;

#define MINIMUM_AMOUNT_OF_BLOCKS 100
#define TINY_MAX 256 // 2 ^ 8 
#define SMALL_MAX 65536 // 2 ^ 16

#define ALLIGNMENT 16   // must be power of 2


void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);
void show_alloc_mem();
void show_alloc_mem_ex();

void *unsafe_realloc(zone_t **zones, void *ptr, size_t size);
void unsafe_free(zone_t **zones, void *ptr);
void *unsafe_malloc(zone_t **zones, size_t size);


size_t allign_size(size_t size);
size_t get_size_of_tiny();
size_t get_size_of_small();
zonetype_t get_zone_base_on_size(size_t size);
void *allocate_memory(size_t size);
void deallocate_memory(void *ptr, size_t size);
zone_t *create_zone(size_t size, zonetype_t type);
zone_t *find_free_zone(zone_t *zones, size_t size, zonetype_t type);
void insert_zone(zone_t **zones, zone_t* zone);
block_t *create_block(zone_t *zone, size_t size);

zone_t *get_zone(void *ptr);
void free_block(zone_t **zones, zone_t* zone, block_t* block);
void print_string(const char *str);
void print_allocated(zone_t *zones, int detail);
void realloc_take_next_block(block_t *block, size_t size);
debug_config_t *get_debug(void);
void ft_memset(void *ptr, size_t size, char val);


void print_str(char *str);
void print_hexa(size_t number);
void print_decimal(size_t number);
#endif