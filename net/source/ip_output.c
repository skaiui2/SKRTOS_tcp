#include "ipvar.h"
#include "ether.h"
#include "route.h"
#include "ip.h"
#include "debug.h"

unsigned short	ip_id = 2;

#define	IP_DF 0x4000			/* dont fragment flag */
#define	IP_MF 0x2000			/* more fragments flag */
#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */


int ip_output(struct buf *sk, struct route *rt)
{
    struct _sockaddr_in *sa;
    struct ip_struct *ip;
    int hlen;

    sa = (struct _sockaddr_in *)&(rt->ro_dst);
    ip = (struct ip_struct *)sk->data;
    
    hlen = sizeof(struct ip_struct);

    ip->ip_hl = hlen >> 2;
	ip->ip_v = IPVERSION;
    ip->ip_tos = 0;
    ip->ip_len = htons(sk->data_len);
	ip->ip_id = htons(ip_id++);

    //now no fragment
    ip->ip_off = 0;
    ip->ip_ttl = 64;
	ip->ip_p = sk->type;
    ip->ip_src = OwnerNet.ipaddr;
    ip->ip_dst = sa->sin_addr;
    ip->ip_sum = 0;
    ip->ip_sum = in_checksum(ip, hlen);

    ether_output(&OwnerNet, sk, (struct _sockaddr *)sa);
    return true;
}


