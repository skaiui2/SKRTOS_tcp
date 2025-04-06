#include "ether.h"
#include "arp.h"
#include "mempool.h"
#include "tapif.h"
#include <stdio.h>


void ether_input(struct buf_struct *sk)
{
    struct eth_hdr *eh = (struct eth_hdr *)sk->data_buf;

    switch (ntohs(eh->ether_type))
    {
    case ETH_P_ARP:
        printf("ARP!");
        arp_input(sk);
        
        break;

    case ETH_P_IP:
        printf("IPv4");
        
        break;
    case ETH_P_IPV6:
        printf("ipv6!");
        break;

    
    default:
        break;
    }
    mempool_free(sk);
}

void ether_output(struct buf_struct *sk, uint16_t len)
{
    tapif_output(sk, len);
}


