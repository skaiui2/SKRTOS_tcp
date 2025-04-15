#include "buf.h"
#include "mempool.h"
#include <memory.h>

struct buf_struct *buf_get(uint16_t size)
{
    struct buf_struct *sk = mempool_alloc(pool_buf_handle);
    *sk = (struct buf_struct) {
        .tol_len = STRUCT_SIZE_PRE(struct buf_struct, data_buf) + size,
        .data_len = 0,
        .data = sk->data_buf,
        .type = BUF_DATA,
        .flags = 0
    };
    list_node_init(&(sk->node));
    return sk;
}


void buf_free(struct buf_struct *sk)
{
    mempool_free(sk);
}

void buf_copy(struct buf_struct *sk, void *data_pointer, uint16_t len)
{
    if (len > sizeof(struct buf_struct)) {
        SYS_ERROR("len over MLEN");
    }
    memcpy(sk->data_buf, (const char *)data_pointer, len);

}

void buf_copy_to(void *data_pointer, struct buf_struct *sk, uint16_t len)
{
    if (len > MLEN) {
        SYS_ERROR("len over MLEN");
    }
    memcpy(data_pointer, (void *)sk->data_buf, len);

}
