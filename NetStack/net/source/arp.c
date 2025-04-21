#include "arp.h"
#include "tapif.h"
#include "buf.h"
#include "ifnet.h"
#include "list.h"
#include "debug.h"
#include "in.h"
#include "ether.h"
#include "socket.h"
#include "ether.h"
#include "route.h"
#include "memalloc.h"
#include "ip.h"

struct arp_cache AcHead;
struct list_node ArpInQue;

static unsigned char broadcast_mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};


void arp_init()
{
    list_node_init(&(AcHead.node));
    list_node_init(&(ArpInQue));
    AcHead.rt_ac = &RouteEntry;
}

void arp_cache_add_tail(struct arp_cache *ac)
{
    list_add(&(AcHead.node), &(ac->node));
}

void arp_cache_remove_tail(struct arp_cache *ac)
{
    list_remove(&(ac->node));
}



void arp_InQue_add_tail(struct buf_struct *sk)
{
    list_add(&ArpInQue, &(sk->node));

}

void arp_InQue_remove_tail(struct buf_struct *sk)
{
    list_remove(ArpInQue.prev);
    
}

int arp_resolve(struct arpcom *ac, struct rtentry *rt, struct buf_struct *sk, struct _sockaddr *dst, unsigned char *desten)
{
    struct arp_cache *acc;
    struct list_node *ac_node;
    struct _sockaddr_in *sa;
    struct eth_hdr *eh;
    unsigned int    ipaddr;

    sa = (struct _sockaddr_in *)dst;
    ipaddr = sa->sin_addr.addr;

    eh = (struct eth_hdr *)dst;
    for (ac_node = AcHead.node.next; ac_node != &(AcHead.node); ac_node = ac_node->next) {
        acc = container_of(ac_node, struct arp_cache, node);
        if (acc->ipaddr == sa->sin_addr.addr) {
            memcpy(eh->ether_dhost, acc->hwaddr, 6);
            printf("arp_resolve mac get\r\n");
            print_mac(eh->ether_dhost);
            return 1;
        }
    }

    if (ac_node == &(AcHead.node)) {
        sk->flags = BLOCK;
        arp_request(ac, &(OwnerNet.ipaddr.addr), &ipaddr, desten);        
    }
    return 0;
}

void arp_request(struct arpcom *ac, unsigned int *sip, unsigned int *tip, unsigned char *addr)
{
    struct buf_struct *sk;
    struct eth_hdr *eh;
    struct arp_ether  *ae;
    struct arp_hdr *ah;
    struct _sockaddr sa;

    sk = buf_get(sizeof(struct buf_struct));
    sk->data += sizeof(struct eth_hdr);
    sk->data_len += sizeof(struct arp_ether);

    ae = (struct arp_ether *)sk->data;
    ah = &(ae->ea_hdr);

    ah->ar_hrd = htons(ARPHRD_ETHER);
    ah->ar_pro = htons(ETHERTYPE_IP);
    ah->ar_hln = sizeof(ae->arp_sha);
    ah->ar_pln = sizeof(ae->arp_spa);
    ah->ar_op  = htons(ARPOP_REQUEST);

    memcpy(ae->arp_sha, OwnerNet.hwaddr, 6);
    memcpy(&(ae->arp_spa), sip, 4);

    memset(ae->arp_tha, 0, 6);
    memcpy(&(ae->arp_tpa), tip, 4);

    eh = (struct eth_hdr *)sa.sa_data;
    eh->ether_type = htons(ETHERTYPE_ARP);
	memcpy(eh->ether_dhost, broadcast_mac, 6);

    sa.sa_family = AF_UNSPEC;
    sa.sa_len = sizeof(sa);
    
    ether_output(&OwnerNet, sk, &sa, (struct rtentry *)0);

    buf_free(sk);
}


static void arp_reply(struct buf_struct *sk)
{
    struct _sockaddr sa;
    struct eth_hdr *eh;  
    struct arp_ether *pkt  = (struct arp_ether *)sk->data;

    pkt->ea_hdr.ar_hrd = htons(ARPHRD_ETHER);
    pkt->ea_hdr.ar_pro = htons(ETHERTYPE_IP);
    pkt->ea_hdr.ar_hln = 6;              
    pkt->ea_hdr.ar_pln = 4;            
    pkt->ea_hdr.ar_op  = htons(ARPOP_REPLY); 

    memcpy(pkt->arp_tha, pkt->arp_sha, 6);
    pkt->arp_tpa = pkt->arp_spa;

    memcpy(pkt->arp_sha, OwnerNet.hwaddr, 6);
    pkt->arp_spa = OwnerNet.ipaddr.addr;
   
    eh = (struct eth_hdr *)sa.sa_data;
	memcpy(eh->ether_dhost, pkt->arp_tha, 6);
	eh->ether_type = htons(ETHERTYPE_ARP);
	sa.sa_family = AF_UNSPEC;
	sa.sa_len = sizeof(sa);

    ether_output(&OwnerNet, sk, &sa, (struct rtentry *)0);  
}


void arp_input()
{ 
    struct buf_struct *sk;
    struct arp_ether *ap;
    struct arp_hdr *ah;
    struct list_node *ai_node;
    struct ip_struct *ip;
    struct eth_hdr *eh;
    register struct list_node *ac_node;
    register struct arp_cache *ac;

    ai_node = ArpInQue.next;
    while(ai_node != &ArpInQue) 
    {
        sk  = container_of(ai_node, struct buf_struct, node);
        ac_node = ai_node->next;
        list_remove(ai_node);
        ai_node = ac_node;
        
        ap = (struct arp_ether *)sk->data;
        ah = (struct arp_hdr *)&(ap->ea_hdr);

        for (ac_node = AcHead.node.next; ac_node != &(AcHead.node); ac_node = ac_node->next) {
            ac = container_of(ac_node, struct arp_cache, node);
            if (ac->ipaddr == ap->arp_spa) {
                break;
            }
        }
        if (ac_node == &(AcHead.node)) {
            ac = heap_malloc(sizeof(struct arp_cache));
            ac->ipaddr = ap->arp_spa;
            arp_cache_add_tail(ac);
        }

        for(ac_node = EthOutQue.next; ac_node != &EthOutQue; ac_node = ac_node->next) {
            sk = container_of(ac_node, struct buf_struct, node);
            ip = (struct ip_struct *)sk->data;
            eh = (struct eth_hdr *)(sk->data - sizeof(struct eth_hdr));
            if (ip->ip_dst.addr == ap->arp_spa) {
                memcpy(eh->ether_dhost, ap->arp_sha, 6);
                sk->flags = READY;
            }
        }
        memcpy(ac->hwaddr, ap->arp_sha, 6);
        printf("arp_input mac record\r\n");
        print_mac(ac->hwaddr);
        
        switch (ntohs(ah->ar_op))
        {
        case ARPOP_REQUEST:
            arp_reply(sk);
        break;
        case ARPOP_REPLY:

        break;

        default:
        break;
        }
    }
    

}




void arp_timer()
{

}




void arp_free()
{

}


