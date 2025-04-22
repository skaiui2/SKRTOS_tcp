#ifndef INPCB_H
#define INPCB_H
#include "ip.h"
#include "buf.h"
#include "socket_var.h"



struct inpcb {
    struct list_node node;	
	struct	_in_addr inp_faddr;	
	unsigned short	inp_fport;		
	struct	_in_addr inp_laddr;	
	unsigned short	inp_lport;
	struct	buf_struct *sk;
    sem_t sem;
	int inp_protocol;

	struct	socket *inp_socket;
	struct	route inp_route;
	int		inp_flags;		
	struct	ip_struct inp_ip;		
	struct	ip_moptions *inp_moptions; 
};


#endif 
