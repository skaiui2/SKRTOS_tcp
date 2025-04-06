#ifndef ARP_H
#define APR_H

#include "macro.h"
#include "list.h"
#include "ether.h"

struct arp_hdr {
    unsigned short ar_hrd;
    unsigned short ar_pro;
    unsigned char  ar_hln;
    unsigned char  ar_pln;
    unsigned short ar_op;
}__attribute__((packed));

struct arp_ether {
    struct arp_hdr ea_hdr;
    unsigned char arp_sha[6];
    uint32_t arp_spa;
    unsigned char arp_tha[6];
    uint32_t arp_tpa;
}__attribute__((packed));

struct arp_cache {
    struct list_node node;
    struct buf_struct *last_buf;
    long count_asked;
};


struct arp_packet {
    struct eth_hdr eh;
    struct arp_ether    ae;

}__attribute__((packed));


void arp_request();

void arp_input(struct buf_struct *sk);
void arp_timer();
void arp_free();
void arp_resolve();


#endif
