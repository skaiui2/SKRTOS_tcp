#include "udp.h"
#include "udp_var.h"
#include "debug.h"
#include "ether.h"
#include "socket_var.h"
#include "inpcb.h"
#include "route.h"

void udp_input(struct buf_struct *sk, int iphlen)
{
    struct udpiphdr *ui;
    struct udphdr *uh;
    struct ip_struct *ip;
    struct ip_struct save_ip;
    unsigned short udp_len;
    unsigned char *payload;

    ip = (struct ip_struct *)(sk->data - sizeof(struct ip_struct));
    save_ip = *ip;
	
    if (iphlen < sizeof(struct ip_struct)) {
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
		} else {
            printf("right!\r\n");
        }
	}
	*ip = save_ip;
	ip->ip_src.addr = htonl(ip->ip_src.addr);

	sk->data += sizeof(struct udphdr);
	sk->data_len -= sizeof(struct udphdr);

    udp_output(0, sk, 0, 0); 
}



int udp_output(struct inpcb *inp, struct buf_struct *sk, struct buf_struct  *addr, struct buf_struct *control)
{
    register struct udpiphdr *ui;
	int len;;
	struct route rt;
	struct sock_addr_in *sa;
	struct ip_struct *ip;
	int s, error = 0;

	len = sk->data_len;
	
	ui = (struct udpiphdr *)(sk->data - sizeof(struct udpiphdr));
	ip = (struct ip_struct *)ui;

	print_ip(ip->ip_src.addr);
	
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
	print_ip(ui->ui_i.ih_src.addr);
	ui->ui_i.ih_dst.addr = htonl(ui->ui_i.ih_src.addr);
	ui->ui_i.ih_src = OwnerNet.ipaddr;
	ui->ui_u.uh_dport = ui->ui_u.uh_sport;
	ui->ui_u.uh_sport = htons(1234);

	ui->ui_u.uh_ulen = ui->ui_i.ih_len;


	ui->ui_u.uh_sum = 0;
	if ((ui->ui_u.uh_sum = in_checksum(ui, sizeof (struct udpiphdr) + len)) == 0) {
		ui->ui_u.uh_sum = 0xffff;
	}
    
	sa = (struct sock_addr_in *)&rt.ro_dst;
    sa->sin_family = AF_INET;
    sa->sin_addr.addr = ui->ui_i.ih_dst.addr;
	print_ip(sa->sin_addr.addr);
    sa->sin_len = sizeof(*sa);

	printf("udpout\r\n");
	print_content((char *)sk->data_buf, 60);

	sk->data -= sizeof(struct udpiphdr);
	sk->data_len += sizeof(struct udpiphdr);
	sk->type = ip->ip_p;
    ip_output(sk, &rt);
	
	return (error);

}

