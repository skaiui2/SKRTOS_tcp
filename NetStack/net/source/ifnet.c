#include "ifnet.h"

struct ifnet OwnerNet;

void parse_mac_address(const char *mac, unsigned char hwaddr[6]) {
    char mac_copy[18]; 
    strncpy(mac_copy, mac, sizeof(mac_copy) - 1);
    mac_copy[sizeof(mac_copy) - 1] = '\0'; 

    char *token = strtok(mac_copy, ":"); 
    for (int i = 0; i < 6 && token != NULL; i++) {
        hwaddr[i] = (unsigned char)strtol(token, NULL, 16);
        token = strtok(NULL, ":");
    }
}



void ifnet_init(char *ip, char *mac, uint16_t mtu)
{
    struct ip_addr addr;
    inet_pton(AF_INET, ip, &addr);

    struct ifnet *net = &OwnerNet;
    *net = (struct ifnet) {
        .ipaddr = addr,
        .mtu = mtu,
    };
    parse_mac_address(mac, net->hwaddr); 

}

