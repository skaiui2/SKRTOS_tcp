#include "icmp.h"
#include "buf.h"
#include "ip.h"
#include "ether.h"
#include "debug.h"


void icmp_send(struct buf_struct *sk, struct buf_struct *send_sk)
{
    uint16_t len = send_sk->data_len - sizeof(struct eth_hdr) - sizeof(struct ip_struct) - sizeof(struct icmp);
    memcpy(send_sk->data, sk->data, len);
    printf("send\r\n");
    print_content(send_sk->data_buf, 100);
    ip_output(send_sk);
}


void icmp_reflect(struct buf_struct *sk, struct buf_struct *send_sk)
{
    struct icmp *icp = (struct icmp *)sk->data;
    struct icmp *send_icp = (struct icmp *)send_sk->data;
    *send_icp = *icp;
    send_icp->icmp_type = ICMP_ECHOREPLY;
    send_icp->icmp_cksum = 0;
    send_icp->icmp_cksum = checksum((void *)send_icp, sk->data_len);

    sk->data += sizeof(struct icmp);
    send_sk->data += sizeof(struct icmp);


    icmp_send(sk, send_sk);

}

void icmp_input(struct buf_struct *sk, struct buf_struct *send_sk)
{
    struct icmp *icp = (struct icmp *)sk->data;
    print_content((char *)icp, sizeof(struct icmp));

    switch (icp->icmp_type)
    {
    case ICMP_ECHO:
        printf("icmp echo!\r\n");
        
        icmp_reflect(sk, send_sk);
        
        break;
    
    default:
        break;
    }

}


void icmp_error()
{

}


