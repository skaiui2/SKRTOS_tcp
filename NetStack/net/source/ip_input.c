#include "ip.h"
#include "icmp.h"


void ip_input(struct buf_struct *sk, struct buf_struct *send_sk)
{
    struct ip_struct *ip = (struct ip_struct *)sk->data;

    if (ip->ip_v != IPVERSION) {
        SYS_ERROR("It is not IPv4\r\n");
        return;
    }

    unsigned short csum = -1;
    csum = checksum((void *)ip, (ip->ip_hl) << 2); 
    if (csum != 0) {
        SYS_ERROR("check sum error!\r\n");
    }
    struct ip_struct *send_ip = (struct ip_struct *)send_sk->data;
    *send_ip = *ip;
    send_ip->ip_dst = ip->ip_src;
    send_ip->ip_src = ip->ip_dst;
    send_ip->ip_sum = 0;
    send_ip->ip_sum = checksum((void *)send_ip, send_ip->ip_hl << 2);

    sk->data += sizeof(struct ip_struct);
    send_sk->data  += sizeof(struct ip_struct);
    send_sk->data_len = sk->data_len;
    sk->data_len = ip->ip_len - ip->ip_hl << 2;


    switch (ip->ip_p) 
    {
    case IPPROTO_ICMP:
        printf("ICMP!\r\n");
        icmp_input(sk, send_sk); 
        break;
    
    default:
        break;
    }
}


