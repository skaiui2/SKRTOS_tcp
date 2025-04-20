
#include "tapif.h"
#include "mempool.h"
#include "arp.h"
#include "ether.h"
#include "ifnet.h"
#include "debug.h"
#include "ip.h"
#include "tcp.h"


void net_init()
{
    pool_buf_handle = mempool_creat(sizeof(struct buf_struct), 20);
    eth_init();
    arp_init();
    ip_init();
    tcp_init();

    ifnet_init("192.168.1.200", "9e:4d:9e:e3:48:9f", 150);
}


int main() {
    
    int ret = 0;
    fd_set readfds; 

    fd = tapif_init();
    net_init();

    while(1) {
        ret = select(fd + 1, &readfds, NULL, NULL, NULL);
        if(ret == 1) {
            ether_input();
        }

    }
}


