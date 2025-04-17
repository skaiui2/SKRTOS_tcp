#ifndef IP_H
#define IP_H
#include "buf.h"
#include "route.h"
#include "ipvar.h"


#define IPVERSION	4

extern struct list_node IpInQue;

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
    struct in_addr_r  ip_src;
    struct in_addr_r  ip_dst; 

}__attribute__((packed));



void ip_init();
void ip_input();


int ip_output(struct buf_struct *sk, struct buf_struct *opt, struct route *ro, int flags, struct ip_moptions *imo);

void ip_forward(struct buf_struct *sk, int  opts);




#endif
