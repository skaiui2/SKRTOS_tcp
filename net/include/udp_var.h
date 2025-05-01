#ifndef UDP_VAR_H
#define UDP_VAR_H

#include "udp.h"


struct	udpiphdr {
	struct 	ipovly ui_i;		
	struct	udphdr ui_u;		
}__attribute__((packed));







#endif

