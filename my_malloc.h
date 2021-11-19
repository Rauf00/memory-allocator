#ifndef _MY_MALLOC_H_
#define _MY_MALLOC_H_
#include <stdbool.h>
void* mem_init();
void* my_malloc(size_t reqSize);
void my_free(void* ptr);
void* my_malloc_cleanup();

#endif