#include "arp.h"
#include "tapif.h"
#include "buf.h"
#include "memalloc.h"
#include "ifnet.h"
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>

struct arp_cache ac;

void arp_init()
{
    list_head_init((struct list_head *)&(ac.node));
}

void arp_request()
{
    struct buf_struct *sk = buf_get();
    uint8_t *data_arp = buf_data_ptr_add(sk, 128 - sizeof(struct arp_ether));
    
    struct arp_ether *arp = (struct arp_ether *)data_arp;
    struct arp_hdr *ah = (struct arp_hdr *)arp;
    *ah = (struct arp_hdr) {
        .ar_hrd = htons(ARPHRD_ETHER),
        .ar_pro = htons(ETHERTYPE_IP),
        .ar_hln = sizeof(arp->arp_sha),
        .ar_pln = sizeof(arp->arp_spa),
        .ar_op  = htons(ARPOP_RREQUEST)
    };

}


#include "debug.h"
static void arp_reply(struct buf_struct *sk)
{
    struct arp_packet *ap = (struct arp_packet *)sk->data_buf;
    struct arp_packet *pkt = heap_malloc(sizeof(struct arp_packet));

    memcpy(pkt->eh.ether_dhost, ap->ae.arp_sha, 6);
    memcpy(pkt->eh.ether_shost, OwnerNet.hwaddr, 6);
    pkt->eh.ether_type = ap->eh.ether_type; 
    
    pkt->ae.ea_hdr.ar_hrd = htons(ARPHRD_ETHER);
    pkt->ae.ea_hdr.ar_pro = ap->ae.ea_hdr.ar_pro;
    pkt->ae.ea_hdr.ar_hln = 6;              
    pkt->ae.ea_hdr.ar_pln = 4;            
    pkt->ae.ea_hdr.ar_op  = htons(ARPOP_REPLY); 
    memcpy(pkt->ae.arp_sha, OwnerNet.hwaddr, 6);
    pkt->ae.arp_spa = OwnerNet.ipaddr.addr;
    memcpy(pkt->ae.arp_tha, ap->ae.arp_sha, 6);
    pkt->ae.arp_tpa = ap->ae.arp_spa;

    print_content((char *)pkt, sizeof(struct arp_packet));

    tapif_output((struct buf_struct *)pkt, sizeof(struct arp_packet));   
    heap_free(pkt);
}


void arp_input(struct buf_struct *sk)
{ 
    struct arp_packet *ap = (struct arp_packet *)sk->data_buf;
    struct arp_hdr *ah = (struct arp_hdr *)&(ap->ae.ea_hdr);
    
   // while(ac.node.next) 
   {

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


void arp_resolve()
{

}


