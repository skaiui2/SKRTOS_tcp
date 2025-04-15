#ifndef ETHER_H
#define ETHER_H

#include "buf.h"
#include "in.h"
#include "ifnet.h"
#include "socket.h"
#include "route.h"

struct eth_hdr {
    unsigned char   ether_dhost[6];
    unsigned char   ether_shost[6];
    unsigned short  ether_type;
}__attribute__((packed));


struct	arpcom {
	struct 	ifnet ac_if;		/* network-visible interface */
	unsigned char	ac_enaddr[6];		/* ethernet hardware address */
	struct	in_addr_r ac_ipaddr;	/* copy of ip address- XXX */
	struct	ether_multi *ac_multiaddrs; /* list of ether multicast addrs */
	int	ac_multicnt;		/* length of ac_multiaddrs list */	
};


struct ether_multi {
	unsigned char	enm_addrlo[6];		/* low  or only address of range */
	unsigned char	enm_addrhi[6];		/* high or only address of range */
	struct	arpcom *enm_ac;		/* back pointer to arpcom */
	unsigned int	enm_refcount;		/* no. claims to this addr/range */
	struct	ether_multi *enm_next;	/* ptr to next ether_multi */
};



void ether_input();

void ether_output(struct ifnet *ifp, struct buf_struct *sk, struct sock_addr *dst, struct rtentry *rt);


#endif
