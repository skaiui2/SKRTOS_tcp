#include "tcp.h"
#include "ip.h"
#include "tcpip.h"
#include "ifnet.h"
#include "debug.h"

struct list_node TcpInQue;

void tcp_init()
{
    list_node_init(&TcpInQue);
}



void tcp_input(struct buf_struct *sk, int iphlen)
{
    struct tcpiphdr *ti;
    unsigned short csum;
    struct ip_struct *ip;
    struct ip_struct save_ip;
    int len;
    int tlen;

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

    tcp_output(0, sk, 0);

}

#define TCP_CLOSED         0
#define TCP_LISTEN         1
#define TCP_SYN_SENT       2
#define TCP_SYN_RECEIVED   3
#define TCP_ESTABLISHED    4
#define TCP_FIN_WAIT_1     5
#define TCP_FIN_WAIT_2     6
#define TCP_CLOSE_WAIT     7
#define TCP_CLOSING        8
#define TCP_LAST_ACK       9
#define TCP_TIME_WAIT      10


int tcp_output(struct tcpcb *tp, struct buf_struct *sk, struct _sockaddr  *sc)
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


    /*
     if (ti->ti_t.th_flags & 0x02) {
        ti->ti_t.th_flags |= 0x10;
        ti->ti_t.th_ack = htonl(ntohl(ti->ti_t.th_seq) + 1);
	    ti->ti_t.th_seq = htonl(75863);
    }
    */

    if (ti->ti_t.th_flags & 0x02) { 
        ti->ti_t.th_flags = 0x12;
        ti->ti_t.th_ack = htonl(ntohl(ti->ti_t.th_seq) + 1);
        ti->ti_t.th_seq = htonl(12345678);
    }

    ti->ti_t.th_sum = 0;
    ti->ti_t.th_sum = in_checksum(ti, sk->data_len);

    sa = (struct _sockaddr_in *)&rt.ro_dst;
    sa->sin_family = AF_INET;
    sa->sin_addr.addr = ti->ti_i.ih_dst.addr;
	print_ip(sa->sin_addr.addr);
    sa->sin_len = sizeof(*sa);

    sk->type = 0x06;
	printf("tcpout\r\n");
    
    ip_output(sk, &rt);

}