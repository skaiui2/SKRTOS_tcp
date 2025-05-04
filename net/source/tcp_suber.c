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
	int win = 64240;
	struct route *ro;

    printf("FLAGS:%d\r\n", flags);
 
    if (tp) {
        win = sbspace(&tp->t_inpcb->inp_socket->so_rcv);
        ro = &tp->t_inpcb->inp_route;
    } 
    win =2140;

    ti = (struct tcpiphdr *)sk->data;

    ti->ti_i.ih_next = ti->ti_i.ih_prev = 0;
	ti->ti_i.ih_x1 = 0;
	ti->ti_i.ih_pr = IPPROTO_TCP;
	ti->ti_i.ih_len = htons(sk->data_len - sizeof(struct ip_struct));

    ti->ti_i.ih_dst = tp->t_inpcb->inp_faddr;

    ti->ti_i.ih_src = tp->t_inpcb->inp_laddr;

    ti->ti_t.th_dport = tp->t_inpcb->inp_fport;
    ti->ti_t.th_sport = tp->t_inpcb->inp_lport;
	
	ti->ti_t.th_seq = htonl(seq);
	ti->ti_t.th_ack = htonl(ack);
	ti->ti_t.th_x2 = 0;
	ti->ti_t.th_off = (sk->data_len - sizeof(struct ip_struct) - sk->tol_len) >> 2;
	ti->ti_t.th_flags = flags;
	if (tp)
		ti->ti_t.th_win = htons((unsigned short) (win >> tp->rcv_scale));
	else
		ti->ti_t.th_win = htons((unsigned short)win);

    ti->ti_t.th_urp = 0;
    ti->ti_t.th_sum = 0;
    ti->ti_t.th_sum = in_checksum(ti, sk->data_len);

    sa = (struct _sockaddr_in *)&(ro->ro_dst);
    sa->sin_family = AF_INET;
    sa->sin_addr.addr = ti->ti_i.ih_dst.addr;
    sa->sin_len = sizeof(*sa);

    sk->type = IPPROTO_TCP;
	printf("tcp respond\r\n");
    
    ip_output(sk, ro);
}



struct tcpcb *tcp_close(struct tcpcb *tp)
{
    struct tcpiphdr *t;
    struct inpcb *inp = tp->t_inpcb;
    struct socket *so = inp->inp_socket;
    struct buf *sk;
    struct rtentry *rt;
    unsigned int i;

    rt = inp->inp_route.ro_rt;
    if (rt->rt_rmx.rmx_rtt) {
        rt->rt_rmx.rmx_rtt = (rt->rt_rmx.rmx_rtt + tp->t_srtt * RTT_CALC_FACTOR) >> 1;
    } else {
        rt->rt_rmx.rmx_rtt = tp->t_srtt * RTT_CALC_FACTOR;
    }

    if (rt->rt_rmx.rmx_rttvar) {
        rt->rt_rmx.rmx_rttvar = (rt->rt_rmx.rmx_rttvar + tp->t_rttvar * RTT_CALC_FACTOR_VAR) >> 1;
    } else {
        rt->rt_rmx.rmx_rttvar = tp->t_rttvar * RTT_CALC_FACTOR_VAR;
    }

    if (tp->t_maxseg & (tp->t_maxseg - 1)) {
        i = (tp->snd_ssthresh + tp->t_maxseg >> 1) / tp->t_maxseg;
    } else {
        i = (tp->snd_ssthresh + tp->t_maxseg >> 1) & (~tp->t_maxseg);
    }

    if (i < 2) {
        i = 2;
    }
    i *= (unsigned int)(tp->t_maxseg + sizeof(struct tcpiphdr));

    if (rt->rt_rmx.rmx_ssthresh) {
        rt->rt_rmx.rmx_ssthresh = (rt->rt_rmx.rmx_ssthresh + i) >> 1;
    } else {
        rt->rt_rmx.rmx_ssthresh = i;
    }

    inp->inp_ppcb = NULL;
    return NULL;
}




struct tcpcb *tcp_drop(struct tcpcb *tp, int errno)
{
    struct socket *so = tp->t_inpcb->inp_socket;

    if(tp->t_state == TCP_SYN_RECEIVED) {
        tp->t_state = TCP_CLOSED;
        struct buf *sk = buf_get(0);
        tcp_output(tp, sk);
    }

    return tcp_close(tp);
}

void tcp_send_syn(struct tcpcb *tp, struct buf *sk) 
{
    sk->data -= sizeof(struct tcpiphdr); 
    sk->data_len += (sizeof(struct tcpiphdr) + 4);

    struct tcpiphdr *ti = (struct tcpiphdr *)sk->data;
    unsigned char *tcp_options = (unsigned char *)(ti + 1); 

    tcp_options[0] = 0x02; 
    tcp_options[1] = 0x04; 
    tcp_options[2] = (TCP_MSS >> 8) & 0xFF; 
    tcp_options[3] = TCP_MSS & 0xFF; 
    tp->t_state = TCP_SYN_SENT;
    tcp_respond(tp, sk, 0, 560, TH_SYN);
}


void tcp_receive_syn_ack(struct tcpcb *tp, struct tcpiphdr *ti) 
{
    if (ti->ti_t.th_flags & TH_ACK) {
        tp->rcv_nxt = ntohl(ti->ti_t.th_ack); 
        tp->t_state = TCP_ESTABLISHED;   
    }
}

void tcp_send_ack(struct tcpcb *tp, struct buf *sk) 
{
    tcp_respond(tp, sk, tp->rcv_nxt, tp->snd_una, TH_ACK);
}


void tcp_send_fin(struct tcpcb *tp, struct buf *sk) 
{
    printf("fin ack:%u\r\n", tp->rcv_nxt);
    printf("fin seq:%u\r\n", tp->snd_nxt);

    tp->t_state = TCP_FIN_WAIT_1;
    tcp_respond(tp, sk, tp->rcv_nxt, tp->snd_nxt, (TH_FIN | TH_ACK));
}


void tcp_receive_ack(struct tcpcb *tp, struct buf *sk) 
{
    struct tcpiphdr *ti = (struct tcpiphdr *)sk->data;
    if (ti->ti_t.th_flags & TH_ACK) {
        tp->t_state = TCP_FIN_WAIT_2;
        tcp_send_ack(tp, sk); 
    }
}


void tcp_receive_fin(struct tcpcb *tp, struct buf *sk) 
{
    struct tcpiphdr *ti = (struct tcpiphdr *)sk->data;
    if (ti->ti_t.th_flags & TH_FIN) {
        tp->t_state = TCP_TIME_WAIT;
        tcp_send_ack(tp, sk); 
    }
}


