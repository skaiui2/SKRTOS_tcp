#include "inpcb.h"
#include "memalloc.h"





int in_pcballoc(struct socket *so, struct list_node *head)
{
	struct inpcb *inp = heap_malloc(sizeof(struct inpcb));
	if (inp == NULL) {
		SYS_ERROR("socket NULL!!!");
		return ERROR;
	}
	*inp = (struct inpcb) {
		.inp_socket = so,
	};
	list_node_init(&(inp->node));
	list_add(head, &inp->node);
	so->so_pcb = (void *)inp;	
	return 0;
}


int in_pcbfree(struct inpcb *inp)
{
	list_remove(&inp->node);
	heap_free(inp);
}

struct inpcb *in_pcblookup(struct list_node *head, struct _in_addr faddr, struct _in_addr laddr, 
						unsigned int fport, unsigned int lport)
{
	struct list_node *inp_node; 
	struct inpcb *inp = NULL;
	struct inpcb *match_pcb = NULL;
	char count = 0;
	char max_match = 0;
	for(inp_node = head->next; inp_node != head; inp_node = inp_node->next) {
		inp = container_of(inp_node, struct inpcb, node);
		if ((lport == INADDR_ANY) || 
			(inp->inp_lport == lport)) {
			count++;
		} 
		if ((inp->inp_laddr.addr == INADDR_ANY) || 
			(inp->inp_laddr.addr == laddr.addr)) {
			count++;
		}

		if (inp->inp_fport == fport) {
			count++;
		}
		if (inp->inp_faddr.addr == faddr.addr) {
			count++;
		}

		if (count > max_match) {
			max_match = count;
			match_pcb = inp;
		}
		count = 0;
	}

	if (match_pcb == NULL) {
		SYS_ERROR("inpcb no find!");
	}

	return match_pcb;
}


int in_pcbbind(struct inpcb *inp, struct _sockaddr_in *sin)
{
	if (sin->sin_family != AF_INET) {
		SYS_ERROR("sin_family is error!");
		return ERROR;
	}

	inp->inp_laddr = sin->sin_addr;
	inp->inp_lport = sin->sin_port;  
}

int in_pcbconnect(struct inpcb *inp, struct _sockaddr_in *fsin)
{
	if (fsin->sin_family != AF_INET) {
		SYS_ERROR("sin_family is error!");
		return ERROR;
	}
	
	if (fsin->sin_port == 0) {
		SYS_ERROR("port is 0");
		return ERROR;
	}
	
	inp->inp_faddr = fsin->sin_addr;
	inp->inp_fport = fsin->sin_port;
	return	0;
}

int in_pcbdisconnect(struct inpcb *inp)
{
	inp->inp_faddr.addr = INADDR_ANY;
	inp->inp_fport = 0;
	return true;
}

int in_setsockaddr(struct inpcb *inp, struct _sockaddr_in *lsin)
{
	lsin->sin_family = AF_INET;
	lsin->sin_len = sizeof(*lsin);
	lsin->sin_port = inp->inp_lport;
	lsin->sin_addr = inp->inp_laddr;
}

int in_setpeeraddr(struct inpcb *inp, struct _sockaddr_in *fsin)
{
	fsin->sin_family = AF_INET;
	fsin->sin_len = sizeof(*fsin);
	fsin->sin_port = inp->inp_fport;
	fsin->sin_addr = inp->inp_faddr;
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


