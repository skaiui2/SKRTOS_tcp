#include "netdev.h"
#include <string.h>
#include <stdio.h>
int netdev_init(struct netdev *dev) {
    
    memset(dev, 0, sizeof(*dev));

    strncpy(dev->name, "tap1", sizeof(dev->name) - 1);

    dev->hwaddr[0] = 0x00;
    dev->hwaddr[1] = 0x11;
    dev->hwaddr[2] = 0x22;
    dev->hwaddr[3] = 0x33;
    dev->hwaddr[4] = 0x44;
    dev->hwaddr[5] = 0x55;

    dev->ipaddr[0] = 192;
    dev->ipaddr[1] = 168;
    dev->ipaddr[2] = 1;
    dev->ipaddr[3] = 200;

    printf("Network device initialized:\n");
    printf("Name: %s\n", dev->name);
    printf("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           dev->hwaddr[0], dev->hwaddr[1], dev->hwaddr[2],
           dev->hwaddr[3], dev->hwaddr[4], dev->hwaddr[5]);
    printf("IP: %d.%d.%d.%d\n",
           dev->ipaddr[0], dev->ipaddr[1], dev->ipaddr[2], dev->ipaddr[3]);

    return 0;
}

