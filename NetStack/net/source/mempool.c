

#include "mempool.h"
#include "memalloc.h"
#include "macro.h"


Class(PoolNode)
{
    uint8_t used;
    PoolNode *next;
};

Class(PoolHead)
{
    PoolNode *head;
    size_t BlockSize;
    size_t AllCount;
    uint8_t RemainNode;
};

PoolHeadHandle pool_buf_handle;

static const size_t NodeStructSize = (sizeof(PoolNode) + (size_t)(alignment_byte)) &~(alignment_byte);
static const size_t HeadStructSize = (sizeof(PoolHead) + (size_t)(alignment_byte)) &~(alignment_byte);



void pool_apart(PoolHead *ThePool, uint8_t amount,size_t apart_size)
{
    PoolNode *prev_node;
    PoolNode *new_node;

    prev_node = ThePool->head;
    while(amount != 0) {
        new_node = (PoolNode *) (((size_t) prev_node) + apart_size);
        new_node->used = 0;
        prev_node->next = new_node;
        prev_node = new_node;
        amount--;
    }
    new_node->next = NULL;
}


PoolHeadHandle mempool_creat(uint16_t size,uint8_t amount)
{
    size_t apart_size = size;
    uint32_t all_size;
    void *start_address;
    PoolHead *ThePool;

    apart_size += NodeStructSize;
    if((apart_size & alignment_byte) != 0x00) {
        apart_size += alignment_byte;
        apart_size &= ~(alignment_byte); 
    }

    all_size = (uint32_t)( amount *  ((uint32_t)apart_size));
    start_address = heap_malloc(all_size);
    if (start_address == NULL){
        return false;
    }
    ThePool = (PoolHead *)start_address;
    *ThePool = (PoolHead){
        .head = (PoolNode *)((size_t)start_address + HeadStructSize),
        .BlockSize = size,
        .AllCount = amount,
        .RemainNode = amount
    };
    ThePool->head->used = 0;
    pool_apart(ThePool,amount - 1,apart_size);
    return ThePool;
}


void *mempool_alloc(PoolHeadHandle ThePool)
{
    PoolNode *use_node;
    void *xReturn = NULL;
    use_node = ThePool->head;
    //the next is valid ?
    while(((use_node->used) != 0 ) && (use_node->next != NULL)) {
        use_node = use_node->next;
    }
    if(use_node->used == 0) {
        use_node->used = 1;
        ThePool->RemainNode -= 1;
        xReturn = (void *) (((uint8_t *) use_node) + NodeStructSize);
    }
    return xReturn;
}



void mempool_free(void *xRet)
{
    PoolNode *FreeBlock;
    void * xFree = (void*)((size_t)xRet - NodeStructSize);
    FreeBlock = (void*)xFree;
    FreeBlock->used = 0;
}

void mempool_delete(PoolHeadHandle ThePool)
{
    heap_free(ThePool);
}


