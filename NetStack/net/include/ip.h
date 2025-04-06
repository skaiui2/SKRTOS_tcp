#ifndef IP_H
#define IP_H
#include "buf.h"



struct ip_addr {
    unsigned long   addr;
};


struct ip_struct {
    unsigned char   ip_v:4;
    unsigned char   ip_hl:4;
    unsigned char   ip_tos;
    short           ip_len;
    unsigned short  ip_id;
    short           ip_off;
    unsigned char   ip_ttl;
    unsigned char   ip_p;
    unsigned short  ip_sum;
    struct ip_addr  ip_src, ip_dst; 

};


unsigned short checksum(void *b, int len);


int ip_output(struct buf_struct *sk, struct buf_struct *opts);




#endif
