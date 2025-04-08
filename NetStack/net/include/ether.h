#ifndef ETHER_H
#define ETHER_H

#include "macro.h"
#include "buf.h"

struct eth_hdr {
    unsigned char   ether_dhost[6];
    unsigned char   ether_shost[6];
    unsigned short  ether_type;
}__attribute__((packed));

void ether_input();
void ether_output(struct buf_struct *sk);



#endif
