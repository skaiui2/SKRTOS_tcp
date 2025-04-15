#ifndef IN_VAR_H
#define IN_VAR_H
#include "ip.h"
#include "ifnet.h"


struct in_multi {
	struct	in_addr_r inm_addr;	/* IP multicast address */
	struct	ifnet *inm_ifp;		/* back pointer to ifnet */
	struct	in_ifaddr *inm_ia;	/* back pointer to in_ifaddr */
	unsigned int	inm_refcount;		/* no. membership claims by sockets */
	unsigned int	inm_timer;		/* IGMP membership report timer */
	struct	in_multi *inm_next;	/* ptr to next multicast address */
};



struct in_ifaddr {
	struct	ifaddr ia_ifa;		/* protocol-independent info */
					/* ia_{,sub}net{,mask} in host order */
	unsigned long	ia_net;			/* network number of interface */
	unsigned long	ia_netmask;		/* mask of net part */
	unsigned long	ia_subnet;		/* subnet number, including net */
	unsigned long	ia_subnetmask;		/* mask of subnet part */
	struct	in_addr_r ia_netbroadcast; /* to recognize net broadcasts */
	struct	in_ifaddr *ia_next;	/* next in list of internet addresses */
	struct	sock_addr_in ia_addr;	/* reserve space for interface name */
	struct	sock_addr_in ia_dstaddr; /* reserve space for broadcast addr */
	struct	sock_addr_in ia_sockmask; /* reserve space for general netmask */
	struct	in_multi *ia_multiaddrs; /* list of multicast addresses */
};






#endif

