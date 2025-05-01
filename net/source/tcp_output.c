#include "tcp.h"
#include "buf.h"
#include "route.h"
#include "tcpip.h"
#include "ifnet.h"
#include "ip.h"
#include "debug.h"
#include "tcp_fsm.h"
#include "macro.h"

#define TCP_NSTATES 11

unsigned char	tcp_outflags[TCP_NSTATES] = {
    TH_RST|TH_ACK, 0, TH_SYN, TH_SYN|TH_ACK,
    TH_ACK, TH_ACK,
    TH_FIN|TH_ACK, TH_FIN|TH_ACK, TH_FIN|TH_ACK, TH_ACK, TH_ACK,
};

int tcp_output(struct tcpcb *tp, struct buf *sk)
{
    struct tcpiphdr *ti;
    struct socket *so = tp->t_inpcb->inp_socket;
	struct route rt;
	struct _sockaddr_in *sa;
    int flags, len;
    unsigned int off, win;

	if (tp->snd_max == tp->snd_una && tp->t_idle >= tp->t_rxtcur) {
		tp->snd_cwnd = tp->t_maxseg;
	}

	off = tp->snd_nxt - tp->snd_una;
	win = min(tp->snd_nxt, tp->snd_cwnd);

    flags = tcp_outflags[tp->t_state];

    if (tp->t_force) {
        if (win == 0) {
            printf("win == 0\r\n");
        }
    }

    len = min(so->so_snd.sb_cc, win) - off;
    if (len < 0) {
        len = 0;
        tp->snd_nxt = tp->snd_una;
    }
    if (len > tp->t_maxseg) {
        len = tp->t_maxseg;
    }

    if (COMPARE_GT(tp->snd_una + so->so_snd.sb_cc, tp->snd_nxt)) {
        flags &= ~TH_FIN;
    }
    win = sbspace(&so->so_rcv);



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