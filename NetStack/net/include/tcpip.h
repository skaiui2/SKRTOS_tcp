#ifndef TCPIP_H
#define TCPIP_H
#include "ipvar.h"
#include "tcp.h"





struct tcpiphdr {
    struct ipovly   ti_i;
    struct tcphdr   ti_t;
};




#endif

