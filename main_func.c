
#include "malloc.h"


zone_t *g_zones = NULL;












void *malloc(size_t size) {
    zone_t *zone = NULL;
    block_t *block = NULL;

    size_t alligned_size = allign_size(size);
    zonetype_t type = get_zone_base_on_size(alligned_size);
    
    zone = find_free_zone(alligned_size, type);
    if (!zone) {
        zone = create_zone(alligned_size, type);
        if (!zone) {
            return NULL;
        }
        insert_zone(&g_zones, zone);
    }
    block = create_block(zone);
    return (void *)(block + 1); 
}