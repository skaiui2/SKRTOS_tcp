#include "icmp.h"
#include "buf.h"
#include "ip.h"
#include "ether.h"



void icmp_send(struct buf_struct *sk, struct buf_struct *opts)
{
    ip_output(sk, opts);
}


void icmp_reflect(struct buf_struct *sk)
{
    struct buf_struct *opts = NULL;

    icmp_send(sk, opts);

}

void icmp_input(struct buf_struct *sk)
{
    struct icmp *icp = (struct icmp*)sk->data;

    switch (icp->icmp_type)
    {
    case ICMP_ECHO:
        icmp_reflect(sk);
        
        break;
    
    default:
        break;
    }

}


void icmp_error()
{

}




void construct_ping_reply(int fd, const unsigned char *src_mac, const unsigned char *dst_mac, uint32_t src_ip, uint32_t dst_ip) {
    char buffer[1500]; 
    memset(buffer, 0, sizeof(buffer));

    struct eth_hdr *eth = (struct eth_hdr *)buffer;
    memcpy(eth->ether_dhost, dst_mac, 6); 
    memcpy(eth->ether_shost, src_mac, 6); 
    eth->ether_type = htons(ETH_P_IP); 

    // 构造 IP 头部
    struct ip_struct *ip = (struct ip_struct *)(buffer + sizeof(struct eth_hdr));
    *ip = (struct ip_struct) {
        .ip_v   = 4,
        .ip_hl  = 5,
        .ip_tos = 0,
        .ip_len = htons(sizeof(struct ip_struct) + sizeof(struct icmp)),
        .ip_id  = htons(54321),
        .ip_off = 0,
        .ip_ttl = 64, 
        .ip_p   = 1,
        .ip_sum = checksum(ip, sizeof(struct ip_struct)),
        .ip_src = src_ip,
        .ip_dst = dst_ip,
    };

    // 构造 ICMP 头部
    struct icmp *icp = (struct icmp *)(buffer + sizeof(struct eth_hdr) + sizeof(struct ip_struct));
    *icp = (struct icmp) {
        .icmp_type = ICMP_ECHOREPLY,
        .icmp_code = 0,
        .icmp_cksum = checksum(icp, sizeof(struct icmp))
    };
    
}
