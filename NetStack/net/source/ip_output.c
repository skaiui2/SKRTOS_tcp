#include "ipvar.h"
#include "ether.h"
#include "route.h"
#include "ip.h"

int ip_output(struct buf_struct *sk, struct buf_struct *opt, struct route *ro, int flags, struct ip_moptions *imo)
{
    struct sock_addr_in sa;
    struct ip_struct *ip;

    ip = (struct ip_struct *)sk->data;
   
    sa.sin_family = AF_INET;
    sa.sin_len = sizeof(sa);
    sa.sin_addr = ip->ip_dst; 

    ether_output(&OwnerNet, sk, (struct sock_addr *)&sa, (struct rtentry *)0);
    return true;
}


