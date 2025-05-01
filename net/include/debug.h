#ifndef DEBUG_H
#define DEBUG_H

#include "macro.h"
#include "buf.h"

void print_buf_content(struct buf *buf, int len);

void print_content(char *buf, uint16_t len);


void print_mac(unsigned char mac[6]);

void print_ip(unsigned int ip);

#endif

