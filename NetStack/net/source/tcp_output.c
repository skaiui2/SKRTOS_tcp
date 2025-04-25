#include "tcp.h"
#include "buf.h"
#include "route.h"
#include "tcpip.h"
#include "ifnet.h"
#include "ip.h"
#include "debug.h"
#include "tcp_fsm.h"



int tcp_output(struct tcpcb *tp, struct buf *sk, struct _sockaddr  *sc)
{
    struct tcpiphdr *ti;
    struct _sockaddr_in *addr;
	struct route rt;
	struct _sockaddr_in *sa;

    int len = sk->data_len;
	sk->data -= sizeof(struct tcpiphdr);
	sk->data_len += sizeof(struct tcpiphdr);

    ti = (struct tcpiphdr *)sk->data;

    ti->ti_i.ih_next = ti->ti_i.ih_prev = 0;
	ti->ti_i.ih_x1 = 0;
	ti->ti_i.ih_pr = IPPROTO_TCP;
	ti->ti_i.ih_len = htons((unsigned short)len + sizeof (struct tcphdr));
	printf("ti.:%d\r\n", htons(ti->ti_i.ih_len));

    ti->ti_i.ih_dst = ti->ti_i.ih_src;
    ti->ti_i.ih_src = OwnerNet.ipaddr;

    ti->ti_t.th_dport = ti->ti_t.th_sport;
    ti->ti_t.th_sport = htons(1234);
 
    if (ti->ti_t.th_flags & TH_SYN) {
        ti->ti_t.th_flags |= TH_ACK;
        ti->ti_t.th_ack = htonl(ntohl(ti->ti_t.th_seq) + 1);
	    ti->ti_t.th_seq = htonl(75863);
    }
    
    ti->ti_t.th_sum = 0;
    ti->ti_t.th_sum = in_checksum(ti, sk->data_len);

    sa = (struct _sockaddr_in *)&rt.ro_dst;
    sa->sin_family = AF_INET;
    sa->sin_addr.addr = ti->ti_i.ih_dst.addr;
	print_ip(sa->sin_addr.addr);
    sa->sin_len = sizeof(*sa);

    sk->type = IPPROTO_TCP;
	printf("tcpout\r\n");
    
    ip_output(sk, &rt);

}