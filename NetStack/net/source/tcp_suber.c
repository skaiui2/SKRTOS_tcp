#include "tcp.h"
#include "tcpip.h"
#include "route.h"
#include "ifnet.h"
#include "tcp_fsm.h"
#include "debug.h"



void tcp_respond(struct tcpcb *tp, struct buf *sk, tcp_seq ack, tcp_seq seq, int flags)
{
    struct tcpiphdr *ti;
	struct _sockaddr_in *sa;
    register int tlen;
	int win = 0;
	struct route ro;

    int len = sk->data_len;

    ti = (struct tcpiphdr *)sk->data;

    ti->ti_i.ih_next = ti->ti_i.ih_prev = 0;
	ti->ti_i.ih_x1 = 0;
	ti->ti_i.ih_pr = IPPROTO_TCP;
	ti->ti_i.ih_len = htons((unsigned short)len + sizeof (struct tcphdr));
	printf("ti.:%d\r\n", htons(ti->ti_i.ih_len));

    ti->ti_i.ih_dst = tp->t_inpcb->inp_faddr;
    ti->ti_i.ih_src = tp->t_inpcb->inp_laddr;

    ti->ti_t.th_dport = tp->t_inpcb->inp_fport;
    ti->ti_t.th_sport = tp->t_inpcb->inp_lport;

    ti->ti_i.ih_len = htons((unsigned short)(sizeof (struct tcphdr) + tlen));
	tlen += sizeof (struct tcpiphdr);
	
	
	ti->ti_t.th_seq = htonl(seq);
	ti->ti_t.th_ack = htonl(ack);
	ti->ti_t.th_x2 = 0;
	ti->ti_t.th_off = sizeof (struct tcphdr) >> 2;
	ti->ti_t.th_flags = flags;
	if (tp)
		ti->ti_t.th_win = htons((unsigned short) (win >> tp->rcv_scale));
	else
		ti->ti_t.th_win = htons((unsigned short)win);

    ti->ti_t.th_urp = 0;
    ti->ti_t.th_sum = 0;
    ti->ti_t.th_sum = in_checksum(ti, sk->data_len);

    sa = (struct _sockaddr_in *)&(ro.ro_dst);
    sa->sin_family = AF_INET;
    sa->sin_addr.addr = ti->ti_i.ih_dst.addr;
	print_ip(sa->sin_addr.addr);
    sa->sin_len = sizeof(*sa);

    sk->type = IPPROTO_TCP;
	printf("tcpout\r\n");
    
    ip_output(sk, &ro);
}




