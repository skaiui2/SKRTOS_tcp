#include "ether.h"
#include "arp.h"
#include "mempool.h"
#include "tapif.h"
#include "debug.h"
#include "ip.h"
#include "ifnet.h"


void ether_input()
{
    struct buf_struct *sk = tapif_input();
    if (sk == NULL) {
        SYS_ERROR("SK none!");
        return;
    }
    
    struct buf_struct *send_sk = buf_get(sizeof(struct eth_hdr));
    struct eth_hdr *send_eh = (struct eth_hdr *)send_sk->data; 
    struct eth_hdr *eh = (struct eth_hdr *)sk->data;


    memcpy(send_eh->ether_dhost, eh->ether_shost, 6);
    memcpy(send_eh->ether_shost, OwnerNet.hwaddr, 6);
    send_eh->ether_type = eh->ether_type; 
    

    sk->data += sizeof(struct eth_hdr);
    send_sk->data += sizeof(struct eth_hdr);


    switch (ntohs(eh->ether_type))
    {
    case ETH_P_ARP:

        printf("ARP!\r\n");
        arp_input(sk, send_sk);
        
        break;

    case ETH_P_IP:
        printf("IPv4\r\n");
        ip_input(sk, send_sk);
        
        break;
    case ETH_P_IPV6:
        printf("ipv6!\r\n");
        break;

    
    default:
        break;
    }
    buf_free(sk);
    buf_free(send_sk);
}

void ether_output(struct buf_struct *sk)
{
    tapif_output(sk, sk->data_len);
}


