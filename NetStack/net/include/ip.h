#ifndef IP_H
#define IP_H
#include "buf.h"
#include "ether.h"

#define IPVERSION	4


struct ip_addr {
    unsigned int   addr;
}__attribute__((packed));


struct ip_struct {
    /*LITTLE_ENDIAN!!!*/
    unsigned char   ip_hl:4;
    unsigned char   ip_v:4;
    unsigned char   ip_tos;
    short           ip_len;
    unsigned short  ip_id;
    short           ip_off;
    unsigned char   ip_ttl;
    unsigned char   ip_p;
    unsigned short  ip_sum;
    struct ip_addr  ip_src;
    struct ip_addr  ip_dst; 

}__attribute__((packed));



unsigned short checksum(void *b, int len);


void ip_input(struct buf_struct *sk, struct buf_struct *send_sk);

int  ip_output(struct buf_struct *sk);

void ip_forward(struct buf_struct *sk, int  opts);




#endif
