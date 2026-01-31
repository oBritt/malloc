#include "malloc.h"
#include <unistd.h>
#include <inttypes.h>

int main() {

    void *ptr[4]; 
    for (int i = 0; i < 4; i++) {
        if (i == 1) {
            ptr[i] = malloc(580);
            continue;
        }
        ptr[i] = malloc(290);
    }
    show_alloc_mem_ex();
    free(ptr[1]);
    show_alloc_mem_ex();
    ptr[0] = realloc(ptr[0], 500);
    show_alloc_mem_ex();
}