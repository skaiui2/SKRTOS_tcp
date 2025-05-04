#include "tcp.h"
#include "ip.h"
#include "tcpip.h"
#include "ifnet.h"
#include "debug.h"
#include "tcp_fsm.h"
#include "memalloc.h"
#include <pthread.h>

sem_t sem_con;

struct list_node TcpInpcb;

/*
void tcp_init()
{
    list_node_init(&TcpInpcb);
}
*/


void tcp_init()
{
    list_node_init(&TcpInpcb);
    sem_init(&sem_con, 0, 0);

    struct _sockaddr_in *address = heap_malloc(sizeof(struct _sockaddr_in));
    struct socket *so = heap_malloc(sizeof(struct socket));
    in_pcballoc(so, &TcpInpcb);
    struct inpcb *inp = so->so_pcb;
    address->sin_family = AF_INET;          
    address->sin_addr = OwnerNet.ipaddr; 
    address->sin_port = htons(1234);     
    in_pcbbind(inp, address);
    heap_free(address);

    struct tcpcb *tp = heap_malloc(sizeof(struct tcpcb));
    *tp = (struct tcpcb) {
        .t_state = TCP_LISTEN,
        .t_inpcb = inp,
    };
    inp->inp_ppcb = (void *)tp;
}


void tcp_input(struct buf *sk, int iphlen)
{
    struct tcpiphdr *ti;
    unsigned short csum;
    struct ip_struct *ip;
    struct ip_struct save_ip;
    int len, tlen, off;

    sk->data -= iphlen;
    sk->data_len += iphlen;

    ti = (struct tcpiphdr *)sk->data;
    ip = (struct ip_struct *)ti;
    save_ip = *ip;
	
    if (iphlen < sizeof(struct ip_struct)) {
		SYS_ERROR("iphlen error!");
        return; 
    }

    ip->ip_len = ntohs(ip->ip_len);
    len = ip->ip_len;
    
    tlen = ip->ip_len - iphlen;
    
    ti->ti_i.ih_next = ti->ti_i.ih_prev = 0;
    ti->ti_i.ih_x1 = 0;
    ti->ti_i.ih_len = htons(tlen);
    ti->ti_i.ih_src = ip->ip_src; 

    csum = -1;
    csum = in_checksum((void *)ti, len); 
    if (csum != 0) {
        printf("csum:%d\r\n", csum);
        SYS_ERROR("tcp input check sum error!\r\n");
    }

    *ip =save_ip;

    off = ti->ti_t.th_off << 2;
    if ((off < sizeof(struct tcphdr)) || (off > tlen)) {
        SYS_ERROR("off length error!");
        goto free;
    }
    tlen -= off;
    ti->ti_i.ih_len = tlen;


    struct inpcb *inp = in_pcblookup(&TcpInpcb, ip->ip_src, ip->ip_dst, ti->ti_t.th_sport, ti->ti_t.th_dport);
    struct tcpcb *tp = inp->inp_ppcb;
    printf("tp_stat:%d\r\n", tp->t_state);


    struct _sockaddr_in *sin;
    unsigned int ack;
    unsigned int seq;
    int flags = ti->ti_t.th_flags;
    printf("flags:%d\r\n", flags);

    switch (tp->t_state)
    {
    case TCP_LISTEN:
        if (flags & TH_RST)
			goto free;
		if (flags & TH_ACK)
			goto free;
		if ((flags & TH_SYN) == 0)
			goto free;

        sin = heap_malloc(sizeof(struct _sockaddr_in));
		sin->sin_family = AF_INET;
		sin->sin_len = sizeof(*sin);
		sin->sin_addr = ti->ti_i.ih_src;
		sin->sin_port = ti->ti_t.th_sport;
        
		struct _in_addr laddr = inp->inp_laddr;
		if (inp->inp_laddr.addr == INADDR_ANY)
			inp->inp_laddr = ti->ti_i.ih_dst;
		if (in_pcbconnect(inp, sin)) {
			inp->inp_laddr = laddr;
			heap_free(sin);
			goto free;
		}

        ack = ntohl(ti->ti_t.th_seq) + 1;
        seq = 75863; 
        flags |= TH_ACK;
        printf("listen\r\n");
        tcp_respond(tp, sk, ack, seq, flags);
         
    break;

    case TCP_SYN_SENT:
        if (flags & TH_RST) {
            goto free;
        }
        
        if (flags & TH_ACK) { 
            ack = ntohl(ti->ti_t.th_seq) + 1;
            seq = ntohl(ti->ti_t.th_ack); 

            printf("syn_sent\r\n");
            tp->rcv_nxt = ack;
            tp->snd_nxt = seq;
            tcp_respond(tp, sk, ack, seq, TH_ACK);
            sem_post(&sem_con);
            break;
        }

        if ((flags & TH_SYN) == 0) {
            goto free;
        }
   
    break;

    case TCP_ESTABLISHED:
        if (flags == (TH_PUSH | TH_ACK)) {
            sk->tol_len = sk->data_len - iphlen - off;
            inp->inp_fport = ti->ti_t.th_sport;
            inp->sk = sk;
            void *data = sk->data + iphlen + off;
            inp->recv_data = heap_malloc(sk->tol_len);
            inp->recv_len = sk->tol_len;
            memcpy(inp->recv_data, data, inp->recv_len);
    
            ack = ntohl(ti->ti_t.th_seq) + sk->tol_len;
            seq = ntohl(ti->ti_t.th_ack); 

            printf("ack:%u\r\n", ack);
            printf("seq:%u\r\n", seq);

            printf("TCP_EST PUSH and ACK\r\n");
            tp->rcv_nxt = ack;
            tp->snd_nxt = seq;

            sk->tol_len = 0;
            sk->data_len = iphlen + off;
            tcp_respond(tp, sk, ack, seq, TH_ACK);

            sem_post(&(inp->sem));
        }

        if (flags == (TH_ACK | TH_FIN)) {
            ack = ntohl(ti->ti_t.th_seq) + 1;
            printf("ack:%d\r\n", ack);

            seq = ntohl(ti->ti_t.th_ack); 
            printf("seq:%d\r\n", seq);

            printf("TH_FIN\r\n");
            tp->rcv_nxt = ack;
            tp->snd_nxt = seq;
            tcp_respond(tp, sk, ack, seq, TH_ACK);

            tcp_respond(tp, sk, ack, seq, TH_FIN);
        }

    break;


    case TCP_FIN_WAIT_1:
        
        ack = ntohl(ti->ti_t.th_seq) + 1;
        seq = ntohl(ti->ti_t.th_ack); 

        printf("FIN_WAIT1\r\n");
        tp->rcv_nxt = ack;
        tp->snd_nxt = seq;

        sk->tol_len = 0;
        sk->data_len = iphlen + off;
        if (flags == (TH_FIN | TH_ACK)) {
            tcp_respond(tp, sk, ack, seq, TH_ACK);
        } else {
            goto free;
        }
        

    break;
    default:

    break;
    }

free: 
    buf_free(sk);

}
