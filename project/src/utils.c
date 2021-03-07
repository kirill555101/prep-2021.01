#include <string.h>

#include "utils.h"

size_t custom_strlen(const char* str) {
    return strlen(str);
}

int custom_pow(int base, int power) {
    int res = 1;

    for (int i = 0; i < power; i++) {
        res *= base;
    }

    return res;
}
