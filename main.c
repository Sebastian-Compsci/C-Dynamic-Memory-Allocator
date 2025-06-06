#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "mem.h"

void print_block(const char* label, void* ptr, size_t size) {
    printf("%s (%p): ", label, ptr);
    uint8_t* p = (uint8_t*)ptr;
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", p[i]);
    }
    printf("\n");
}

int main() {
    printf("Testing new_malloc\n");
    void* ptr1 = new_malloc(16);
    if (ptr1) {
        printf("Allocated ptr1 = %p\n", ptr1);
        memset(ptr1, 0xAA, 16);
        print_block("ptr1 after memset", ptr1, 16);
    }

    printf("\nTesting new_calloc\n");
    void* ptr2 = new_calloc(8, 2);  // 16 bytes
    if (ptr2) {
        printf("Allocated ptr2 = %p\n", ptr2);
        //if correct all zeros
        print_block("ptr2 after calloc", ptr2, 16);
    }

    printf("\nTesting new_free\n");
    if (ptr1) {
        new_free(ptr1);
        printf("Freed ptr1\n");
    }
    if (ptr2) {
        new_free(ptr2);
        printf("Freed ptr2\n");
    }

    printf("\nTesting reuse of freed memory\n");
    void* ptr3 = new_malloc(16);
    if (ptr3) {
        printf("Allocated ptr3 = %p\n", ptr3);
        memset(ptr3, 0x55, 16);
        print_block("ptr3 after memset", ptr3, 16);
    }

    if (ptr3) {
        new_free(ptr3);
        printf("Freed ptr3\n");
    }

    printf("Done\n");

    return 0;
}
