#ifndef IPVAR_H
#define IPVAR_H

#include "config.h"
#include "in.h"



struct ipovly {
	void 	*ih_next;
    void    *ih_prev;	
	unsigned char	ih_x1;		
	unsigned char	ih_pr;		
	short	ih_len;			
	struct	in_addr_r ih_src;		
	struct	in_addr_r ih_dst;	
};


struct ip_moptions {
	struct	ifnet *imo_multicast_ifp; /* ifp for outgoing multicasts */
	unsigned char	imo_multicast_ttl;	/* TTL for outgoing multicasts */
	unsigned char	imo_multicast_loop;	/* 1 => hear sends if a member */
	unsigned short	imo_num_memberships;	/* no. memberships this socket */
	struct	in_multi *imo_membership[IP_MAX_MEMBERSHIPS];
};











#endif
