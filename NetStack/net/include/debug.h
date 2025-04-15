#ifndef DEBUG_H
#define DEBUG_H

#include "macro.h"
#include "buf.h"

void print_buf_content(struct buf_struct *buf);

void print_content(char *buf, uint16_t len);


void print_mac(unsigned char mac[6]);

#endif

