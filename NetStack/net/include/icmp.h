#ifndef IP_ICMP_H
#define IP_ICMP_H
#include "macro.h"
#include "ip.h"

struct icmp {
    unsigned char   icmp_type;
    unsigned char   icmp_code;
    unsigned short  icmp_cksum;

    union {
        unsigned char   ih_pptr;
        struct ip_addr  addr;   
        struct ih_idseq {
            unsigned short  icmp_id;
            unsigned short  icmp_seq;
        }idseq;

        struct ih_pmtu {
            short   ipm_void;
            short   ipm_nextmtu;
        }pmtu;

    }icmp_hun;
    
}__attribute__((packed));




void icmp_input(struct buf_struct *sk, struct buf_struct *send_sk);
void icmp_error();
void icmp_reflect();
void icmp_send();


#endif