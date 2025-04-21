#include "ifnet.h"
#include "route.h"
#include "macro.h"
#include "ether.h"
#include "in.h"

struct ifnet OwnerNet;
struct rtentry RouteEntry;


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


/*Now only one mac addr*/
void ifnet_init(char *ip, char *mac, unsigned short mtu)
{
    struct _in_addr addr;
    inet_pton(AF_INET, ip, &addr);

    struct ifnet *net = &OwnerNet;
    *net = (struct ifnet) {
        .ipaddr = addr,
        .mtu = mtu,
    };

    parse_mac_address(mac, net->hwaddr); 

    RouteEntry.rt_ifp = &OwnerNet;

    printf("MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
           net->hwaddr[0], net->hwaddr[1], net->hwaddr[2],
           net->hwaddr[3], net->hwaddr[4], net->hwaddr[5]);

    printf("IP Address: ");
    uint32_t ar = net->ipaddr.addr;
    
    printf("%u.%u.%u.%u",
           ar & 0xFF,
           (ar >> 8) & 0xFF,
           (ar >> 16) & 0xFF,
           (ar >> 24) & 0xFF);
    printf("\n");



}

