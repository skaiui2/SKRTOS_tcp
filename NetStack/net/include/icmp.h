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
        struct in_addr  addr;   
        struct ih_idseq {
            unsigned short  icmp_id;
            unsigned short  icmp_seq;
        }idseq;

        struct ih_pmtu {
            short   ipm_void;
            short   ipm_nextmtu;
        }pmtu;

    }icmp_hun;
    
};


void icmp_input();
void icmp_error();
void icmp_reflect();
void icmp_send();

void construct_ping_reply(int fd, const unsigned char *src_mac, const unsigned char *dst_mac, uint32_t src_ip, uint32_t dst_ip);



#endif