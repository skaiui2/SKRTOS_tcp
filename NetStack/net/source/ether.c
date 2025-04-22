#include "ether.h"
#include "arp.h"
#include "mempool.h"
#include "tapif.h"
#include "debug.h"
#include "ip.h"
#include "ifnet.h"


struct list_node EthOutQue;


void eth_init()
{
    list_node_init(&EthOutQue);
}



void EthOutQue_add_tail(struct buf_struct *sk)
{
    list_add(&EthOutQue, &sk->node);
}

void EthOutQue_remove_tail(struct buf_struct *sk)
{
    list_remove(&sk->node);
}

void ether_input()
{
    struct buf_struct *sk = tapif_input();
    if (sk == NULL) {
        SYS_ERROR("SK none!");
        return;
    }

    struct eth_hdr *eh = (struct eth_hdr *)sk->data;
    if ((memcmp(eh->ether_dhost, OwnerNet.hwaddr, 6) == 0) || 
        (memcmp(eh->ether_dhost, broadcast_mac, 6) == 0)) {
        printf("skbuf input\r\n");
    }else {
        goto freeit;
    }
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

freeit:
    buf_free(sk);
}

void ether_output(struct ifnet *ifp, struct buf_struct *sk, struct _sockaddr *dst, struct rtentry *rt)
{
    struct eth_hdr *eh;
    struct eth_hdr *pkt;
    struct list_node *sk_node;

    EthOutQue_add_tail(sk);
    for(sk_node = EthOutQue.next; sk_node != &EthOutQue; sk_node = sk_node->next) {
        if (sk->flags == BLOCK) {
            continue;
        }
        sk = container_of(sk_node, struct buf_struct, node);

        sk->data -= sizeof(struct eth_hdr);
        sk->data_len += sizeof(struct eth_hdr);
        eh = (struct eth_hdr *)sk->data;

        switch (dst->sa_family)
        {
        case AF_INET:
            if (arp_resolve(0, rt, sk, dst, 0)) {
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
        print_content((char *)sk->data, sk->data_len);

        tapif_output(sk, sk->data_len);
        EthOutQue_remove_tail(sk);
        buf_free(sk);
    }
}


