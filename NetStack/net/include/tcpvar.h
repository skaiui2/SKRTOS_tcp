#ifndef TCPVAR_H
#define TCPVAR_H
#include "macro.h"

struct tcpcb {
    struct  tcpiphdr    *seg_next;
    struct  tcpiphdr    *seg_prev;
    short   t_state;
    short   t_timer[TCPT_NTIMERS];
    short   t_rxtshift;
    short   t_rxtcur;
    short   t_dupacks;
    unsigned short  t_maxseg;
    char    t_force;
    unsigned short  t_flags;


    
};




#endif

