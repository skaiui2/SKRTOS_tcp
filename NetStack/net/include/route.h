#ifndef ROUTE_H
#define ROUTE_H

#include "socket.h"


struct rt_metrics {
	unsigned long	rmx_locks;	/* Kernel must leave these values alone */
	unsigned long	rmx_mtu;	/* MTU for this path */
	unsigned long	rmx_hopcount;	/* max hops expected */
	unsigned long	rmx_expire;	/* lifetime for route, e.g. redirect */
	unsigned long	rmx_recvpipe;	/* inbound delay-bandwith product */
	unsigned long	rmx_sendpipe;	/* outbound delay-bandwith product */
	unsigned long	rmx_ssthresh;	/* outbound gateway buffer limit */
	unsigned long	rmx_rtt;	/* estimated round trip time */
	unsigned long	rmx_rttvar;	/* estimated rtt variance */
	unsigned long	rmx_pksent;	/* packets sent using this route */
};



struct rtentry {
	unsigned long	rt_ip;;		
	unsigned long	rt_mask;
	struct	sockaddr *rt_gateway;	
	short	rt_flags;	
	short	rt_refcnt;	
	unsigned long	rt_use;			
	struct	ifnet *rt_ifp;		
	struct	ifaddr *rt_ifa;		/* the answer: interface to use */
	struct	sockaddr *rt_genmask;	/* for generation of cloned routes */
	void 	*rt_llinfo;		/* pointer to link level info cache */
	struct	rt_metrics rt_rmx;	/* metrics used by rx'ing protocols */
	struct	rtentry *rt_gwroute;	/* implied entry for gatewayed routes */
};


struct route {
	struct	rtentry *ro_rt;
	struct	sock_addr ro_dst;
};







#endif
