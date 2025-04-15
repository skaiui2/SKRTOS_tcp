#ifndef BUF_H
#define BUF_H

#include "macro.h"
#include "config.h"
#include "list.h"

#define BUF_DATA  0
#define BUF_DIRTY 1
#define BUF_META  2
#define BUF_INVAL 3

struct buf_struct {
    struct list_node node; 
    uint16_t    tol_len;
    uint16_t    data_len;
    uint8_t     *data;
    uint8_t     type;
    uint8_t     flags;
    uint8_t     data_buf[MLEN];

};

struct buf_struct *buf_get(uint16_t size);
void buf_free(struct buf_struct *sk);
void buf_copy(struct buf_struct *sk, void *data_poniter, uint16_t len);
void buf_copy_to(void *data_poniter, struct buf_struct *sk, uint16_t len);



#endif

