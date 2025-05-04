#ifndef TCP_H
#define TCP_H
#include "macro.h"
#include "buf.h"
#include "tcpvar.h"
#include "socket.h"

extern struct list_node TcpInpcb;
extern sem_t sem_con;


struct tcphdr {
    unsigned short  th_sport;
    unsigned short  th_dport;
    unsigned int   th_seq;
    unsigned int   th_ack;
    //little ENDIAN!!!
    unsigned char   th_x2:4;
    unsigned char   th_off:4;
    unsigned char   th_flags;
    unsigned short  th_win;
    unsigned short  th_sum;
    unsigned short  th_urp;
}__attribute__((packed));





void tcp_respond(struct tcpcb *tp, struct buf *sk, tcp_seq ack, tcp_seq seq, int flags);

void tcp_init();
void tcp_input(struct buf *sk, int iphlen);


int tcp_output(struct tcpcb *tp, struct buf *sk);


void tcp_send_syn(struct tcpcb *tp, struct buf *sk);
void tcp_receive_syn_ack(struct tcpcb *tp, struct tcpiphdr *ti);
void tcp_send_ack(struct tcpcb *tp, struct buf *sk);
void tcp_send_fin(struct tcpcb *tp, struct buf *sk);

void tcp_receive_fin(struct tcpcb *tp, struct buf *sk);

void tcp_receive_fin(struct tcpcb *tp, struct buf *sk);




#define RTM_RTTUNIT     1000000   
#define PR_SLOWHZ       2         
#define TCP_RTT_SCALE   8    
#define TCP_RTTVAR_SCALE 8

#define RTT_CALC_FACTOR (RTM_RTTUNIT & (~(PR_SLOWHZ * TCP_RTT_SCALE - 1))) 
#define RTT_CALC_FACTOR_VAR (RTM_RTTUNIT & (~(PR_SLOWHZ * TCP_RTTVAR_SCALE - 1)))


#define TCP_MSS 600









#endif

