#include "socket.h"
#include "memalloc.h"
#include "buf.h"
#include "udp.h"
#include "macro.h"


struct list_node InpQue;


struct inpcb *_socket(int inp_protocol)
{
    struct inpcb* inp = heap_malloc(sizeof(struct inpcb));
    list_node_init(&InpQue);
    sem_init(&inp->sem, 0, 0);
    inp->inp_protocol = inp_protocol;
    return inp;
}


int _bind(struct _sockaddr *addr, struct inpcb *inp)
{
    struct _sockaddr_in *s_in = (struct _sockaddr_in *)addr; 
    inp->inp_lport = s_in->sin_port;
    list_add(&InpQue, &inp->node);
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


int _sendto(char *str, int len, struct inpcb * inp, struct _sockaddr *addr)
{

    struct _sockaddr_in *socket;
    struct buf *sk = buf_get(len);
    sk->data += 60;
    sk->data_len = len;

    socket = (struct _sockaddr_in *)addr;
    const char *ip_str = "192.168.1.1";

    if (inet_pton(AF_INET, ip_str, &socket->sin_addr) <= 0) {
        perror("Invalid IP address format");
        exit(EXIT_FAILURE);
    }

    switch (inp->inp_protocol) {
    case IPPROTO_UDP:
        sk->type = IPPROTO_UDP;
        break;
    case IPPROTO_TCP:
        sk->type = IPPROTO_TCP;
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

    memcpy(sk->data, str, len);
    udp_output(0, sk, addr);
}

