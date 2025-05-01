
#ifndef MEMALLOC_H
#define MEMALLOC_H
#include "macro.h"
#include "config.h"

void *heap_malloc(size_t WantSize);
void heap_free(void *xReturn);


#endif
