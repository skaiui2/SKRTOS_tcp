#ifndef SOCKET_H
#define SOCKET_H

#include "in.h"
#include "list.h"
#include "buf.h"

#include <semaphore.h>

extern struct list_node SockQue;


struct _sockaddr {
    unsigned char sa_len;
    unsigned char sa_family;
    char   sa_data[14];
}__attribute__((packed));


struct sock {
    struct list_node node;
    unsigned int port;
    int data_len;
    struct buf_struct *sk;
    void *data;
    sem_t sem;
};

int _socket();

struct sock *_bind(int port);
int _recvfrom(char *str, struct sock *sc, struct _sockaddr *addr);

int _sendto(char *str, int len, struct sock * sc, struct _sockaddr *addr);

#endif
