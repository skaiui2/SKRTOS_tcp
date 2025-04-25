#include "tcp.h"
#include "ip.h"
#include "tcpip.h"
#include "ifnet.h"
#include "debug.h"
#include "tcp_fsm.h"
#include "memalloc.h"

struct list_node TcpInpcb;

void tcp_init()
{
    list_node_init(&TcpInpcb);

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

    ti = (struct tcpiphdr *)(sk->data - iphlen);
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
    } else {
        printf("right\r\n");
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
    
    sk->data += sizeof(struct tcpiphdr) + off - sizeof(struct tcphdr);
    sk->data_len -= sizeof(struct tcpiphdr) + off - sizeof(struct tcphdr);
    
    struct _sockaddr_in *sin;
    int flags = ti->ti_t.th_flags;
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

	    sk->data -= sizeof(struct tcpiphdr)+ off - sizeof(struct tcphdr);
	    sk->data_len += sizeof(struct tcpiphdr)+ off - sizeof(struct tcphdr);
            unsigned int ack = ntohl(ti->ti_t.th_seq) + 1;
            unsigned int seq = 75863; 
            flags = ti->ti_t.th_flags | TH_ACK;
            tcp_respond(tp, sk, ack, seq, flags);
            break;
        

    break;

    case TCP_SYN_SENT:
        if (flags & TH_RST) {
            goto free;
        }
        
        if (flags & TH_ACK) { 
            unsigned int ack = htonl(ntohl(ti->ti_t.th_seq) + 1);
            unsigned int seq = htonl(75863); 
            flags = ti->ti_t.th_flags |= TH_ACK;
            tcp_respond(tp, sk, ack, seq, TH_ACK);
            break;
        }

        if ((flags & TH_SYN) == 0) {
            goto free;
        }
   
    break;

    default:

    break;
    }

free: 
    buf_free(sk);

}
