#ifndef SOCKET_H
#define SOCKET_H

#include "in.h"
#include "list.h"
#include "buf.h"

#include <semaphore.h>

extern struct list_node InpQue;


struct _sockaddr {
    unsigned char sa_len;
    unsigned char sa_family;
    char   sa_data[14];
}__attribute__((packed));




struct inpcb *_socket(int inp_protocol);

int _bind(struct _sockaddr *addr, struct inpcb *inp);
int _recvfrom(char *str, struct inpcb *inp, struct _sockaddr *addr);

int _sendto(char *str, int len, struct inpcb *inp, struct _sockaddr *addr);

#endif

