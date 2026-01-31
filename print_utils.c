#include "malloc.h"

size_t ft_strlen(char *str) {
    size_t out;

    out = 0;
    while (*str) {
        out++;
        str++;
    }
    return out;
}


void print_recursive(size_t number, char* letters) {
    size_t len;
    int mod;

    len = ft_strlen(letters);
    if (number == 0) {
        return;
    }
    mod = number % len;
    print_recursive(number / len, letters);
    write(1, letters + mod, 1);
}


void print_decimal(size_t number) {
    if (number != 0) {
        print_recursive(number, "0123456789");
    } else {
        write(1, "0", 1);
    }
}


void print_hexa(size_t number) {
    write(1, "0x", 2);
    if (number != 0) {
        print_recursive(number, "0123456789ABCDEF");
    } else {
        write(1, "0", 1);
    }
}

void print_string(const char *str) { 
    write(1, str, ft_strlen(str));
}