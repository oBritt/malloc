
#include "malloc.h"

zone_t *find_free_zone(zone_t *zones, size_t size, zonetype_t type) {
    if (!zones) {
        return NULL;
    }

    zone_t *ptr = zones;
    while (ptr) {
        if (type == ptr->type) {
            block_t *block_ptr = ptr->blocks;
            while (block_ptr) {
                if (get_free(block_ptr) && block_ptr->alligned_size >= allign_size(size)) {
                    return ptr;
                }
                block_ptr = block_ptr->next;
            }
        }
        ptr = ptr->next;
    }
    return NULL;
}


void insert_zone(zone_t **zones, zone_t* zone) {
    if (*zones == NULL) {
        *zones = zone;
        return;
    }
    if (*zones > zone) {
        zone->next = *zones;
        (*zones)->prev = zone;
        *zones = zone;
        return;
    }
    zone_t *prev = *zones;
    zone_t *cur = prev->next;
    while (cur)
    {
        if (zone < cur) {
            zone->next = cur;
            prev->next = zone;
            zone->prev = prev;
            cur->prev = zone;
            return;
        }
        prev = cur;
        cur = cur->next;
    }
    prev->next = zone;
    zone->prev = prev;
}
