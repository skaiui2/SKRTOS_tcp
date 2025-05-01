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




void socket_init();
struct inpcb *_socket(int inp_protocol);

int _bind(struct inpcb *inp, struct _sockaddr *addr);
int _connect(struct inpcb *inp, struct _sockaddr *addr);
int _recvfrom(char *str, struct inpcb *inp, struct _sockaddr *addr);

int _sendto(char *str, int len, struct inpcb *inp, struct _sockaddr *addr);

int _shutdown(struct inpcb *inp, int how);

int _close(struct inpcb *inp);


#endif

