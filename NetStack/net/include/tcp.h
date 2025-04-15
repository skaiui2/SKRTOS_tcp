#ifndef TCP_H
#define TCP_H
#include "macro.h"


struct tcphdr {
    unsigned short  th_sport;
    unsigned short  th_dport;
    unsigned long   th_seq;
    unsigned long   th_ack;
    //little ENDIAN!!!
    unsigned char   th_x2:4;
    unsigned char   th_off:4;
    unsigned char   th_flags;
    unsigned short  th_win;
    unsigned short  th_sum;
    unsigned short  th_urp;
};









#endif

