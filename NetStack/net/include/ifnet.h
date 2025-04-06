#ifndef IF_NET_H
#define IF_NET_H
#include "macro.h"
#include "ip.h"

extern struct ifnet OwnerNet;

typedef err_rt (*ifnet_fn)(struct buf_struct *p, struct ifnet *net);
struct ifnet
{
    struct ifnet *next;
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;
    unsigned char hwaddr[6];
    uint16_t mtu;
    void *state;
    ifnet_fn init;
    ifnet_fn input;
};


void ifnet_init(char *ip, char *mac, uint16_t mtu);



#endif

