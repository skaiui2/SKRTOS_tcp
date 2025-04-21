#ifndef INPCB_H
#define INPCB_H
#include "ip.h"
#include "buf.h"
#include "socket_var.h"


struct inpcb {
	struct	inpcb *inp_next;
    struct  inpcb *inp_prev;
	struct	inpcb *inp_head;	
	struct	_in_addr inp_faddr;	
	unsigned short	inp_fport;		
	struct	_in_addr inp_laddr;	
	unsigned short	inp_lport;
	struct	socket *inp_socket;
	void	*inp_ppcb;		
	struct	route inp_route;
	int		inp_flags;		
	struct	ip_struct inp_ip;		
	struct	buf_struct *inp_options;
	struct	ip_moptions *inp_moptions; 
};


#endif 
