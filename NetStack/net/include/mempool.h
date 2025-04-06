#ifndef MEMPOOL_H
#define MEMPOOL_H
#include "macro.h"

typedef struct PoolHead * PoolHeadHandle;

PoolHeadHandle mempool_creat(uint16_t size,uint8_t amount);
void *mempool_alloc(PoolHeadHandle ThePool);
void mempool_free(void *xRet);
void mempool_delete(PoolHeadHandle ThePool);

/*
 * use it for buf_struct.
*/
extern PoolHeadHandle pool_buf_handle;


#endif
