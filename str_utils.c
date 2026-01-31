#include "malloc.h"

void ft_memset(void *ptr, size_t size, char val) {
    char *ptr_char = (char *)ptr;
    
    for (size_t i = 0; i < size; i++) {
        ptr_char[i] = val;
    }
}