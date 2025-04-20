#include "tcp.h"
#include "ip.h"
#include "tcpip.h"


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
    int len;
    int tlen;

    ti = (struct tcpiphdr *)(sk->data - sizeof(struct ip_struct));
    ip = (struct ip_struct *)ti;

    ip->ip_len = ntohs(ip->ip_len);

    tlen = ip->ip_len - sizeof(struct ip_struct);
    len = ip->ip_len;
    
    ti->ti_i.ih_next = ti->ti_i.ih_prev = 0;
    ti->ti_i.ih_x1 = 0;
    ti->ti_i.ih_len = (unsigned short)tlen;
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

    tcp_output(NULL);

}

int tcp_output(struct tcpcb *tp)
{



    ip_output(0, 0);

}