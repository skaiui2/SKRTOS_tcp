#include "socket.h"
#include "memalloc.h"
#include "buf.h"
#include "udp.h"
#include "macro.h"
#include "tcp.h"
#include "tcp_fsm.h"
#include "tcpip.h"

struct list_node InpQue;


void socket_init()
{
    list_node_init(&InpQue);
} 


struct inpcb *_socket(int inp_protocol)
{
    struct socket *so = heap_malloc(sizeof(struct socket));
    
    if (inp_protocol == IPPROTO_TCP) {
        in_pcballoc(so, &TcpInpcb);
    }

    if (inp_protocol == IPPROTO_UDP) {
        in_pcballoc(so, &InpQue);
    }
    
    struct inpcb *inp = so->so_pcb;
    sem_init(&inp->sem, 0, 0);
    inp->inp_protocol = inp_protocol;
    inp->inp_socket = so;

    if (inp_protocol == IPPROTO_TCP) {
        struct tcpcb *tp = heap_malloc(sizeof(struct tcpcb));
        *tp = (struct tcpcb) {
            .t_state = TCP_LISTEN,
            .t_inpcb = inp,
            .t_maxseg = 40,
            .rcv_nxt = htonl(122),
            .snd_una = htonl(156),
        };
        inp->inp_ppcb = (void *)tp;
    }

    return inp;
}


int _bind(struct inpcb *inp, struct _sockaddr *addr)
{
    struct _sockaddr_in *s_in = (struct _sockaddr_in *)addr;
    inp->inp_laddr = s_in->sin_addr; 
    inp->inp_lport = s_in->sin_port;
}


int _connect(struct inpcb *inp, struct _sockaddr *addr)
{
    struct _sockaddr_in *s_in = (struct _sockaddr_in *)addr; 
    inp->inp_faddr = s_in->sin_addr;
    inp->inp_fport = s_in->sin_port;

    if (inp->inp_protocol == IPPROTO_TCP) {
        struct buf *sk = buf_get(0);
        sk->data += 60;
        sk->data_len = 0;
        sk->type = IPPROTO_TCP;
        tcp_send_syn(inp->inp_ppcb, sk);
    }
     
}


int _recvfrom(char *str, struct inpcb *inp, struct _sockaddr *addr)
{  
    struct _sockaddr_in *socket;
    socket = (struct _sockaddr_in *)addr;
    sem_wait(&inp->sem);
    memcpy(str, inp->sk->data, 10);
    socket->sin_addr = inp->inp_laddr;
    socket->sin_port = inp->inp_fport;

    return inp->sk->data_len;
}


int _sendto(char *str, int len, struct inpcb *inp, struct _sockaddr *addr)
{
    struct _sockaddr_in *socket;
    struct buf *sk = buf_get(len);
    sk->data += 60;
    sk->data_len = len;
    sk->tol_len = len;

    socket = (struct _sockaddr_in *)addr;

    memcpy(sk->data, str, len);

    switch (inp->inp_protocol) {
    case IPPROTO_UDP:
        sk->type = IPPROTO_UDP;
        udp_output(0, sk, addr);

        break;
    case IPPROTO_TCP:
    
        sem_wait(&sem_con);
        sk->type = IPPROTO_TCP;
        sk->data -= sizeof(struct tcpiphdr); 
        sk->data_len += sizeof(struct tcpiphdr);
        struct tcpcb *tp = inp->inp_ppcb;
        tcp_respond(tp, sk, tp->rcv_nxt, tp->snd_nxt, TH_ACK);

        break;
    case IPPROTO_ICMP:
        sk->type = IPPROTO_ICMP;
        break;
    case IPPROTO_IPV6:
        sk->type = IPPROTO_IPV6;
        break;
    default:
        sk->type = 0;
    }

}


int _shutdown(struct inpcb *inp, int how)
{

}



int _close(struct inpcb *inp)
{


}

