#ifndef UDP_H
#define UDP_H
#include "macro.h"
#include "ifnet.h"
#include "ipvar.h"
#include "buf.h"
#include "inpcb.h"



struct udphdr {
	unsigned short	uh_sport;	
	unsigned short	uh_dport;		
	unsigned short	uh_ulen;		
	unsigned short	uh_sum;			
}__attribute__((packed));


void udp_input(struct buf_struct *sk, int iphlen);
int udp_output(struct inpcb *inp, struct buf_struct *sk, struct _sockaddr *sc);



#endif

