#ifndef NETDEV_H
#define NETDEV_H

struct netdev {
    char name[16];             
    unsigned char hwaddr[6];   
    unsigned char ipaddr[4];   
};


#endif