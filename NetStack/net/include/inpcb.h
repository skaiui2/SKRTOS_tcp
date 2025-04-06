#ifndef INPCB_H
#define INPCB_H
#include "macro.h"
#include "ip.h"
#include "buf.h"


struct inpcb {
	struct	inpcb *inp_next;
    struct  inpcb *inp_prev;
	struct	inpcb *inp_head;	
	struct	ip_addr inp_faddr;	
	unsigned short	inp_fport;		
	struct	ip_addr inp_laddr;	
	unsigned short	inp_lport;
	void	*inp_ppcb;		
	int	inp_flags;		
	struct	ip_struct inp_ip;		
	struct	buf_struct *inp_options;
};


#endif 
