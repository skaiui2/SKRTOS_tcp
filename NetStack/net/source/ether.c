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

    struct eth_hdr *eh = (struct eth_hdr *)sk->data;

    sk->data += sizeof(struct eth_hdr);
    sk->data_len -= sizeof(struct eth_hdr);

    switch (ntohs(eh->ether_type))
    {
    case ETH_P_ARP:

        printf("ARP!\r\n");
        list_add(&ArpInQue, &(sk->node));
        arp_input();
        
        break;

    case ETH_P_IP:
        printf("IPv4\r\n");
        list_add(&IpInQue, &(sk->node));
        ip_input();
        
        break;
    case ETH_P_IPV6:
        printf("ipv6!\r\n");
        break;

    
    default:
        break;
    }
    buf_free(sk);
}

void ether_output(struct ifnet *ifp, struct buf_struct *sk, struct sock_addr *dst, struct rtentry *rt)
{
    struct eth_hdr *eh;
    struct eth_hdr *pkt;

    sk->data -= sizeof(struct eth_hdr);
    sk->data_len += sizeof(struct eth_hdr);
    eh = (struct eth_hdr *)sk->data;
    switch (dst->sa_family)
    {
    case AF_INET:
        if (arp_resolve(0, rt, 0, dst, 0)) {
            pkt = (struct eth_hdr *)dst;
            memcpy(eh->ether_dhost, pkt->ether_dhost, 6);
            printf("ether_output mac get\r\n");
            print_mac(eh->ether_dhost);
        }	
		eh->ether_type = htons(ETHERTYPE_IP);
        break;
    case AF_UNSPEC:
        pkt = (struct eth_hdr *)dst->sa_data;
        memcpy(eh->ether_dhost, pkt->ether_dhost, 6);
        eh->ether_type = pkt->ether_type;  
        break;
    
    default:
        break;
    } 

    memcpy(eh->ether_shost, ifp->hwaddr, 6);

    printf("ether output len: %d\r\n",sk->data_len);
    print_content((char *)sk->data_buf, sk->data_len);
    tapif_output(sk, sk->data_len);
}


