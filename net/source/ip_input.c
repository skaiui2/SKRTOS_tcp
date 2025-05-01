#include "ip.h"
#include "icmp.h"
#include "list.h"
#include "tcp.h"
#include "udp.h"


struct list_node IpInQue;

void ip_init()
{
    list_node_init(&IpInQue);
}

void ip_InQue_add_tail(struct buf *sk)
{
    list_add(&IpInQue, &(sk->node));

}

void ip_InQue_remove_tail(struct buf *sk)
{
    list_remove(IpInQue.prev);
}




void ip_input()
{
    struct list_node *first_node;
    struct buf *sk;
    struct ip_struct *ip;
    unsigned short csum;
    int hlen;

    first_node = IpInQue.next;
    while(first_node != &IpInQue) {
        list_remove(first_node);

        sk  = container_of(first_node, struct buf, node);
        first_node = first_node->next;

        ip = (struct ip_struct *)sk->data;
        printf("ip len:%d\r\n", ip->ip_len);
        if (ip->ip_v != IPVERSION) {
            SYS_ERROR("It is not IPv4\r\n");
            return;
        }

        csum = -1;
        csum = in_checksum((void *)ip, (ip->ip_hl) << 2); 
        if (csum != 0) {
            SYS_ERROR("ip input check sum error!\r\n");
        }

        sk->data  += sizeof(struct ip_struct);
        sk->data_len -= sizeof(struct ip_struct);

        hlen = ip->ip_hl << 2; 

        switch (ip->ip_p) 
        {
        case IPPROTO_ICMP:
            printf("ICMP!\r\n");
            icmp_input(sk, hlen); 
            break;

        case IPPROTO_UDP:
            printf("udp input\r\n");
            udp_input(sk, hlen);
            break;

        case IPPROTO_TCP:
            printf("tcp input!\r\n");
            tcp_input(sk, hlen);
            break;
    
        default:
            break;
        }
    }
    
}


