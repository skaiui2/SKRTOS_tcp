#ifndef SOCKET_H
#define SOCKET_H



struct sock_addr {
    unsigned char sa_len;
    unsigned char sa_family;
    char   sa_data[14];
};




#endif
