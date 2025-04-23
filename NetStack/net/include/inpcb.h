#ifndef INPCB_H
#define INPCB_H
#include "ip.h"
#include "buf.h"
#include "socket_var.h"
#include "in.h"


struct inpcb {
    struct list_node node;	
	struct	_in_addr inp_faddr;	
	unsigned short	inp_fport;		
	struct	_in_addr inp_laddr;	
	unsigned short	inp_lport;
	struct	buf *sk;
    sem_t sem;
	int inp_protocol;

	struct	socket *inp_socket;
	struct	route inp_route;
	int		inp_flags;		
	struct	ip_struct inp_ip;		
	struct	ip_moptions *inp_moptions; 
};


int in_pcballoc(struct socket *so, struct inpcb *head);
int in_pcbfree(struct inpcb *inp);
struct inpcb *in_pcblookup(struct inpcb *head, struct _in_addr faddr, struct _in_addr laddr, 
							unsigned int fport, unsigned int lport, int flags);
int in_pcbbind(struct inpcb *inp, struct buf *sk);
int in_pcbconnect(struct inpcb *inp, struct buf sk);
int in_pcbdisconnect(struct inpcb *inp);
int in_setsockaddr(struct inpcb *inp, struct buf *sk);
int in_setpeeraddr(struct inpcb *inp, struct buf *sk);
int in_pcbnotify();
void in_rtchange(struct inpcb *inp, int errno);
int in_losing(struct inpcb *inp);

#endif 
