#ifndef MEMH
#define MEMH

#include <stddef.h>
 
void* new_malloc(size_t size);

void new_free(void* ptr);

void* new_calloc(size_t num, size_t size);

#endif
