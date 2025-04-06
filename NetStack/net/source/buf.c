#include "buf.h"
#include "mempool.h"
#include <memory.h>

struct buf_struct *buf_get()
{
    struct buf_struct *sk = mempool_alloc(pool_buf_handle);
    memset(sk, 0, sizeof(struct buf_struct));
    sk->len = sizeof(struct buf_struct) - STRUCT_SIZE_PRE(struct buf_struct, data_buf);
    sk->data = sk->data_buf;
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
    memcpy(data_pointer, (void *)sk, len);

}


uint8_t *buf_data_ptr_add(struct buf_struct *sk, uint16_t len)
{
    uint8_t *data_ptr = sk->data;
    data_ptr += len;
    return data_ptr;
}