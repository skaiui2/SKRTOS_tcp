#include "inpcb.h"

int in_pcballoc(struct socket *so, struct inpcb *head)
{


}


int in_pcbfree(struct inpcb *inp)
{


}

struct inpcb *in_pcblookup(struct inpcb *head, struct _in_addr faddr, struct _in_addr laddr, 
							unsigned int fport, unsigned int lport, int flags)
{


}


int in_pcbbind(struct inpcb *inp, struct buf *sk)
{

    
}

int in_pcbconnect(struct inpcb *inp, struct buf sk)
{


}

int in_pcbdisconnect(struct inpcb *inp)
{


}

int in_setsockaddr(struct inpcb *inp, struct buf *sk)
{


}

int in_setpeeraddr(struct inpcb *inp, struct buf *sk)
{


}


int in_pcbnotify()
{

}

void in_rtchange(struct inpcb *inp, int errno)
{

}

int in_losing(struct inpcb *inp)
{



}


