#include "malloc.h"
#include <unistd.h>
#include <inttypes.h>

int main() {

    void *t = malloc(12321312);
    void *ptr[4]; 
    for (int i = 0; i < 4; i++) {
        ptr[i] = malloc(290 * (1 + i));
    }
    show_alloc_mem();
    free(t);
    show_alloc_mem();
    free(ptr[1]);
    show_alloc_mem();
    free(ptr[2]);
    show_alloc_mem();
    free(ptr[3]);
    show_alloc_mem();
    free(ptr[0]);
    show_alloc_mem();

}