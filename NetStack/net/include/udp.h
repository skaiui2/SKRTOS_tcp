#ifndef UDP_H
#define UDP_H
#include "macro.h"
#include "ifnet.h"
#include "buf.h"
#include "inpcb.h"


struct ipovly {
	void	*ih_next;
    void    *ih_prev;	
	unsigned char	ih_x1;			
	unsigned char	ih_pr;		
	short	ih_len;		
	struct	in_addr ih_src;		
	struct	in_addr ih_dst;		
};



struct udp_hdr {
	unsigned short	uh_sport;	
	unsigned short	uh_dport;		
	short	uh_ulen;		
	unsigned short	uh_sum;			
};


struct	udpiphdr {
	struct 	ipovly ui_i;		
	struct	udp_hdr ui_u;		
};



void udp_input(struct buf_struct *sk, int iplen);
int udp_output(struct inpcb *inp, struct buf_struct *sk, struct buf_struct  *net, struct buf_struct *control);



#endif

