#include "ip.h"


int ip_output(struct buf_struct *sk)
{
    ether_output(sk);
    return true;
}


