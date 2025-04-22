#ifndef ARP_H
#define APR_H


#include "buf.h"
#include "ether.h"
#include "socket.h"

extern struct list_node ArpInQue;
extern unsigned char broadcast_mac[6];




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
    unsigned int arp_spa;
    unsigned char arp_tha[6];
    unsigned int arp_tpa;
}__attribute__((packed));




struct arp_cache {
    struct list_node node;
    //using
    unsigned int  ipaddr;
    unsigned char hwaddr[6];

    // Now no-use
    struct rtentry  *rt_ac; 
    struct buf_struct *last_buf;
    long count_asked;
};



void arp_init();

int arp_resolve(struct arpcom *ac, struct rtentry *rt, struct buf_struct *sk, struct _sockaddr *dst, unsigned char *desten);
void arp_request(struct arpcom *ac, unsigned int *sip, unsigned int *tip, unsigned char *addr);

void arp_input();
void arp_timer();
void arp_free();


#endif
