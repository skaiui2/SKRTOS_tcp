#include "ip.h"
#include "icmp.h"
#include "list.h"


struct list_node IpInQue;

void ip_init()
{
    list_node_init(&IpInQue);
}

void ip_InQue_add_tail(struct buf_struct *sk)
{
    list_add(&IpInQue, &(sk->node));

}

void ip_InQue_remove_tail(struct buf_struct *sk)
{
    list_remove(IpInQue.prev);
}




void ip_input()
{
    struct list_node *first_node;
    struct buf_struct *sk;
    struct ip_struct *ip;

    first_node = IpInQue.next;
    while(first_node != &IpInQue) {
        list_remove(first_node);

        sk  = container_of(first_node, struct buf_struct, node);
        first_node = first_node->next;

        ip = (struct ip_struct *)sk->data;

        if (ip->ip_v != IPVERSION) {
            SYS_ERROR("It is not IPv4\r\n");
            return;
        }

        unsigned short csum = -1;
        csum = checksum((void *)ip, (ip->ip_hl) << 2); 
        if (csum != 0) {
            SYS_ERROR("check sum error!\r\n");
        }

        sk->data  += sizeof(struct ip_struct);
        sk->data_len -= sizeof(struct ip_struct);

        switch (ip->ip_p) 
        {
        case IPPROTO_ICMP:
            printf("ICMP!\r\n");
            icmp_input(sk, ip->ip_hl); 
            break;
    
        default:
            break;
        }
    }
    
}


