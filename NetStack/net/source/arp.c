#include "arp.h"
#include "tapif.h"
#include "buf.h"
#include "ifnet.h"


struct arp_cache ac;

void arp_init()
{
    list_head_init((struct list_head *)&(ac.node));
}

void arp_request()
{
    struct buf_struct *sk = buf_get(sizeof(struct eth_hdr) + sizeof(struct arp_ether));
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
static void arp_reply(struct buf_struct *sk, struct buf_struct *send_sk)
{
    struct arp_ether *ap  = (struct arp_ether *)sk->data;
    struct arp_ether *pkt = (struct arp_ether *)send_sk->data;
    send_sk->data_len = sizeof(struct eth_hdr) + sizeof(struct arp_ether);

    pkt->ea_hdr.ar_hrd = htons(ARPHRD_ETHER);
    pkt->ea_hdr.ar_pro = ap->ea_hdr.ar_pro;
    pkt->ea_hdr.ar_hln = 6;              
    pkt->ea_hdr.ar_pln = 4;            
    pkt->ea_hdr.ar_op  = htons(ARPOP_REPLY); 
    memcpy(pkt->arp_sha, OwnerNet.hwaddr, 6);
    pkt->arp_spa = OwnerNet.ipaddr.addr;
    memcpy(pkt->arp_tha, ap->arp_sha, 6);
    pkt->arp_tpa = ap->arp_spa;

    print_content((char *)send_sk->data_buf, send_sk->data_len);

    ether_output(send_sk);  
}


void arp_input(struct buf_struct *sk, struct buf_struct *send_sk)
{ 
    struct arp_ether *ap = (struct arp_ether *)sk->data;
    struct arp_hdr *ah = (struct arp_hdr *)&(ap->ea_hdr);
    
   // while(ac.node.next) 
   {

        switch (ntohs(ah->ar_op))
        {
        case ARPOP_REQUEST:
            arp_reply(sk, send_sk);
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


