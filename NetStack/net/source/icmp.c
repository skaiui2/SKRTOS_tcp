#include "icmp.h"
#include "buf.h"
#include "ip.h"
#include "ether.h"
#include "debug.h"
#include "route.h"


void icmp_send(struct buf *sk, struct buf *opts)
{
    struct ip_struct *ip;
    struct _sockaddr_in *sa;
    struct route    rt;
    printf("send\r\n");

    sk->data -= sizeof(struct ip_struct);
    sk->data_len += sizeof(struct ip_struct);

    ip = (struct ip_struct *)sk->data;
    sk->type = ip->ip_p;

    sa = (struct _sockaddr_in *)&rt.ro_dst;
    sa->sin_family = AF_INET;
    sa->sin_addr = ip->ip_src;
    sa->sin_len = sizeof(*sa);

    ip_output(sk, &rt);
}


void icmp_reflect(struct buf *sk)
{
    struct buf *opts;
    unsigned short len;
    len = sk->data_len + sizeof(struct ip_struct) + sizeof(struct eth_hdr);
    struct icmp *send_icp = (struct icmp *)sk->data;
    send_icp->icmp_type = ICMP_ECHOREPLY;
    send_icp->icmp_cksum = 0;
    send_icp->icmp_cksum = in_checksum((void *)send_icp, len);

    icmp_send(sk, opts);
}

void icmp_input(struct buf *sk, int hlen)
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


