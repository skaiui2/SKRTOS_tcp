#ifndef IF_NET_H
#define IF_NET_H


#include "in.h"
#include "buf.h"

extern struct ifnet OwnerNet;
extern struct rtentry RouteEntry;


typedef err_rt (*ifnet_fn)(struct buf_struct *p, struct ifnet *net);
struct ifnet {
    struct ifnet *if_next;
    struct list_node *if_addrlist;
    struct in_addr_r ipaddr;
    struct in_addr_r netmask;
    struct in_addr_r gw;
    unsigned char hwaddr[6];
    unsigned short mtu;
    void *state;
    ifnet_fn init;
    ifnet_fn input;
    ifnet_fn output;
};



struct ifaddr {
    struct  ifaddr *ifa_next;
	struct	sock_addr *ifa_addr;	/* address of interface */
	struct	sock_addr *ifa_dstaddr;	/* other end of p-to-p link */
	struct	sock_addr *ifa_netmask;	/* used to determine subnet */
	struct	ifnet *ifa_ifp;		/* back-pointer to interface */
	void	(*ifa_rtrequest)();	/* check or clean routes (+ or -)'d */
	unsigned short	ifa_flags;		/* mostly rt_flags for cloning */
	short	ifa_refcnt;		/* extra to malloc for link info */
	int	ifa_metric;		/* cost of going out this interface */

};



void ifnet_init(char *ip, char *mac, unsigned short mtu);



#endif

