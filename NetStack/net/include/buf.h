#ifndef BUF_H
#define BUF_H

#include "macro.h"
#include "config.h"


struct buf_struct {
    struct buf_struct  *next;
    uint16_t    len;
    uint8_t     *data;
    uint8_t     type;
    uint8_t     flags;
    uint8_t     data_buf[MLEN];

};

struct buf_struct *buf_get();
void buf_free(struct buf_struct *sk);
void buf_copy(struct buf_struct *sk, void *data_poniter, uint16_t len);
void buf_copy_to(void *data_poniter, struct buf_struct *sk, uint16_t len);

uint8_t *buf_data_ptr_add(struct buf_struct *sk, uint16_t len);



#endif

