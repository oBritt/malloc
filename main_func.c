
#include "malloc.h"


zone_t *g_zones = NULL;
pthread_mutex_t g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

void *malloc(size_t size) {
    void *out;
    pthread_mutex_lock(&g_malloc_mutex);
    out = unsafe_malloc(&g_zones, size);
    pthread_mutex_unlock(&g_malloc_mutex);
    return out;
}

void *realloc(void *ptr, size_t size) {
    void *out;
    pthread_mutex_lock(&g_malloc_mutex);
    out = unsafe_realloc(&g_zones, ptr, size);
    pthread_mutex_unlock(&g_malloc_mutex);
    return out;
}

void free(void *ptr) {
    pthread_mutex_lock(&g_malloc_mutex);
    unsafe_free(&g_zones, ptr);
    pthread_mutex_unlock(&g_malloc_mutex);
}

void show_alloc_mem() {
    pthread_mutex_lock(&g_malloc_mutex);
    print_allocated(g_zones, 0);
    pthread_mutex_unlock(&g_malloc_mutex);

}

void show_alloc_mem_ex() {
    pthread_mutex_lock(&g_malloc_mutex);
    print_allocated(g_zones, 1);
    pthread_mutex_unlock(&g_malloc_mutex);

}