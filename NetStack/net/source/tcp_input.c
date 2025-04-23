#include "tcp.h"
#include "ip.h"
#include "tcpip.h"
#include "ifnet.h"
#include "debug.h"
#include "tcp_fsm.h"

struct list_node TcpInQue;

void tcp_init()
{
    list_node_init(&TcpInQue);
}


void tcp_input(struct buf *sk, int iphlen)
{
    struct tcpiphdr *ti;
    register struct tcphdr *tp;
    unsigned short csum;
    struct ip_struct *ip;
    struct ip_struct save_ip;
    int len, tlen, off;

    tp = (struct tcphdr *)sk->data;
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

	sk->data += sizeof(struct tcphdr);
	sk->data_len -= sizeof(struct tcphdr);

    off = ti->ti_t.th_off << 2;
    if ((off < sizeof(struct tcphdr)) || (off > tlen)) {
        SYS_ERROR("off length error!");
        goto free;
    }
    tlen -= off;
    ti->ti_i.ih_len = tlen;

    
    // find PCB
    //struct tcpcb *inp = in_pcblookup();
    

    sk->data += sizeof(struct tcpiphdr) + off - sizeof(struct tcphdr);
    sk->data_len -= sizeof(struct tcpiphdr) + off - sizeof(struct tcphdr);
    
    
    register unsigned char flags;
    flags = tp->th_flags;
    switch (tp->th_flags)
    {
    case TCP_LISTEN:
        struct buf *sk;
        struct _sockaddr_in *sin;
       
        if (flags & TH_RST) {
            goto free;
        }
        
        if (flags & TH_ACK) { 
            //tcp_respond(sk);
            break;
        }

        if ((flags & TH_SYN) == 0) {
            goto free;
        }

    break;

    case TCP_SYN_SENT:
   
    break;

    default:
    break;
    }

free: 
    buf_free(sk);

}
