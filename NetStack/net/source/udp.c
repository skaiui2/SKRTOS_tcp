#include "udp.h"
#include "udp_var.h"
#include "debug.h"
#include "ether.h"

void udp_input(struct buf_struct *sk, int iphlen)
{
    struct udpiphdr *ui;
    struct udphdr *uh;
    struct ip_struct *ip;
    struct ip_struct save_ip;
    unsigned short udp_len;
    unsigned char *payload;

    ip = (struct ip_struc *)(sk->data - sizeof(struct ip_struct));
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
    

}



int udp_output(struct inpcb *inp, struct buf_struct *sk, struct buf_struct  *net, struct buf_struct *control)
{



}

