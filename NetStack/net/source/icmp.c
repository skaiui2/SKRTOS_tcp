#include "icmp.h"
#include "buf.h"
#include "ip.h"
#include "ether.h"
#include "debug.h"


void icmp_send(struct buf_struct *sk, struct buf_struct *opts)
{
    struct ip_struct *ip;
    printf("send\r\n");

    sk->data -= sizeof(struct ip_struct);
    sk->data_len += sizeof(struct ip_struct);

    ip = (struct ip_struct *)sk->data;

    ip->ip_dst = ip->ip_src;
    ip->ip_src = OwnerNet.ipaddr;
    ip->ip_sum = 0;
    ip->ip_sum = in_checksum((void *)ip, ip->ip_hl << 2);

    ip_output(sk, opts, NULL, 0, NULL);
}


void icmp_reflect(struct buf_struct *sk)
{
    struct buf_struct *opts;
    unsigned short len;
    len = sk->data_len + sizeof(struct ip_struct) + sizeof(struct eth_hdr);
    struct icmp *send_icp = (struct icmp *)sk->data;
    send_icp->icmp_type = ICMP_ECHOREPLY;
    send_icp->icmp_cksum = 0;
    send_icp->icmp_cksum = in_checksum((void *)send_icp, len);

    icmp_send(sk, opts);
}

void icmp_input(struct buf_struct *sk, int hlen)
{
    struct icmp *icp;
    icp = (struct icmp *)sk->data;
    print_content((char *)icp, sizeof(struct icmp));

    switch (icp->icmp_type)
    {
    case ICMP_ECHO:
        printf("icmp echo!\r\n");
        
        icmp_reflect(sk);
        
        break;
    
    default:
        break;
    }

}


void icmp_error()
{

}


