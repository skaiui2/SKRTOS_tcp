#include "udp.h"
#include "udp_var.h"
#include "debug.h"
#include "ether.h"
#include "socket_var.h"
#include "inpcb.h"
#include "route.h"
#include "socket.h"


void udp_input(struct buf *sk, int iphlen)
{
    struct udpiphdr *ui;
    struct udphdr *uh;
    struct ip_struct *ip;
    struct ip_struct save_ip;
    unsigned short udp_len;
    unsigned char *payload;

    ip = (struct ip_struct *)(sk->data - iphlen);
    save_ip = *ip;
	
    if (iphlen < sizeof(struct ip_struct)) {
		SYS_ERROR("iphlen error!");
        return; 
    }

    ui = (struct udpiphdr *)ip;
    uh = &(ui->ui_u);

    udp_len = ntohs(uh->uh_ulen);

    if (udp_len < sizeof(struct udphdr) ||
        udp_len > sk->data_len) {
        return; 
    }

	if (uh->uh_sum) {
		((struct ipovly *)ip)->ih_next = 0;
		((struct ipovly *)ip)->ih_prev = 0;
		((struct ipovly *)ip)->ih_x1 = 0;
		((struct ipovly *)ip)->ih_len = uh->uh_ulen;
		if (uh->uh_sum = in_checksum(ip, sk->data_len + sizeof(struct ip_struct))) {
			SYS_ERROR("udp error!!!\r\n");
			return;
		} 
	}
	*ip = save_ip;

	sk->data += sizeof(struct udphdr);
	sk->data_len -= sizeof(struct udphdr);

	
	struct list_node *inp_node;
	for(inp_node = InpQue.next; inp_node != &InpQue; inp_node = inp_node->next) {
		struct inpcb *inp = container_of(inp_node, struct inpcb, node);
		inp->sk = sk;
		inp->inp_fport = uh->uh_sport;
		sem_post(&(inp->sem));
	}
	
}



int udp_output(struct inpcb *inp, struct buf *sk, struct _sockaddr  *sc)
{
    register struct udpiphdr *ui;
	struct _sockaddr_in *addr;
	int len;
	struct route rt;
	struct _sockaddr_in *sa;
	struct ip_struct *ip;
	int error = 0;
	
	printf("udpout\r\n");

	len = sk->data_len;
	ui = (struct udpiphdr *)(sk->data - sizeof(struct udpiphdr));
	ip = (struct ip_struct *)ui;
	

	sk->data -= sizeof(struct udpiphdr);
	sk->data_len += sizeof(struct udpiphdr);
	
	ui->ui_i.ih_next = ui->ui_i.ih_prev = 0;
	ui->ui_i.ih_x1 = 0;
	ui->ui_i.ih_pr = IPPROTO_UDP;
	ui->ui_i.ih_len = htons((unsigned short)len + sizeof (struct udphdr));
	printf("ui.:%d\r\n", htons(ui->ui_i.ih_len));
	/*
	ui->ui_i.ih_src = inp->inp_laddr;
	ui->ui_i.ih_dst = inp->inp_faddr;
	ui->ui_u.uh_sport = inp->inp_lport;
	ui->ui_u.uh_dport = inp->inp_fport;
	*/

	addr = (struct _sockaddr_in *)sc;
	ui->ui_i.ih_dst = addr->sin_addr;
	ui->ui_i.ih_src = OwnerNet.ipaddr;
	ui->ui_u.uh_dport = addr->sin_port;
	ui->ui_u.uh_sport = htons(1234);
	ui->ui_u.uh_ulen = ui->ui_i.ih_len;
	ui->ui_u.uh_sum = 0;
	if ((ui->ui_u.uh_sum = in_checksum(ui, sizeof (struct udpiphdr) + len)) == 0) {
		ui->ui_u.uh_sum = 0xffff;
	}
    
	sa = (struct _sockaddr_in *)&(rt.ro_dst);
    sa->sin_family = AF_INET;
    sa->sin_addr.addr = ui->ui_i.ih_dst.addr;
    sa->sin_len = sizeof(*sa);

    ip_output(sk, &rt);
	
	return error;

}

