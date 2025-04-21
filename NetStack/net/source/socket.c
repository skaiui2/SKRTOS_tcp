#include "socket.h"
#include "memalloc.h"
#include "buf.h"
#include "udp.h"
#include "macro.h"


struct list_node SockQue;

int append(int port, struct buf_struct *sk)
{


}


int _socket()
{
    list_node_init(&SockQue);
}


struct sock *_bind(int port)
{
    struct sock* sc = heap_malloc(sizeof(struct sock));
    sem_init(&sc->sem, 0, 0);
    sc->port = port;
    list_add(&SockQue, &sc->node);
    return sc;
}

int _recvfrom(char *str, struct sock *sc, struct _sockaddr *addr)
{  
    struct _sockaddr_in *socket;
    socket = (struct _sockaddr_in *)addr;
    sem_wait(&sc->sem);
    memcpy(str, sc->data, 10);
    socket->sin_port = sc->port;
    buf_free(sc->sk);

    return sc->data_len;
}


int _sendto(char *str, int len, struct sock * sc, struct _sockaddr *addr)
{

    struct _sockaddr_in *socket;
    struct buf_struct *sk = buf_get(sizeof(len));
    sk->data += 50;
    sk->data_len = len;

    socket = (struct _sockaddr_in *)addr;
    const char *ip_str = "192.168.1.1";

    if (inet_pton(AF_INET, ip_str, &socket->sin_addr) <= 0) {
        perror("Invalid IP address format");
        exit(EXIT_FAILURE);
    }

    sk->type = IPPROTO_UDP;

    /*
    switch () {
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
    */

    memcpy(sk->data, str, len);
    udp_output(0, sk, addr);
}

