#include <mem.h>

void* memcpy(const void* src, void* dest, size_t bytes) {
    const char* s = (char*) src;
    char* d = (char*) dest;

    for (size_t i = 0; i < bytes; i++) {
        d[i] = s[i];
    }

    return dest;
}

void* memset(void* str, int c, size_t bytes) {
    for (size_t i = 0; i < bytes; i++) {
        ((unsigned char*)str)[i] = (unsigned char)c;
    }

    return str;
}

void* memmove(void* dest, const void* src, size_t len) {
    unsigned char cpy[len];

    memcpy(src, cpy, len);

    return memcpy(cpy, dest, len);
}

