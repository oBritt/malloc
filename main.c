#include "malloc.h"
#include <unistd.h>
#include <inttypes.h>


void test_case_realloc1() {
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
    ptr[0] = realloc(ptr[0], 700);
    show_alloc_mem_ex();
    ptr[0] = realloc(ptr[0], 10000);
    show_alloc_mem_ex();
}


void test_realoc2() {
    void *ptr[2000]; 
    for (int i = 0; i < 215; i++) {
        ptr[i] = malloc(250);
    }
    ptr[220] = malloc(32);
    show_alloc_mem_ex();
    ptr[220] = realloc(ptr[220], 100);
    show_alloc_mem_ex();
}

void test_defragmentation() {
    void *ptr[5];
    for (int i = 0; i < 5; i++) {
        ptr[i] = malloc(5000);
    }
    show_alloc_mem_ex();

    free(ptr[1]);
    free(ptr[3]);
    show_alloc_mem_ex();
    free(ptr[2]);
    show_alloc_mem_ex();
    free(ptr[0]);
    free(ptr[4]);
    show_alloc_mem_ex();
}

void test_5() {
    #define M 131072
    malloc(1);
    malloc(2);
    malloc(4);
    malloc(8);
    malloc(16);
    malloc(32);
    malloc(64);
    malloc(128);
    malloc(256);
    malloc(512);
    malloc(1024);
    malloc(1024 * 2);
    malloc(1024 * 4); 
    malloc(1024 * 32);
    malloc(M);
    malloc(16*M);
    malloc(128*M);
    show_alloc_mem(); 
}

int main() {
    dprintf(1, "%ld\n", get_size_of_tiny);
    test_5();

}