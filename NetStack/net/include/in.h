#ifndef IN_H
#define IN_H


struct in_addr_r {
    unsigned int   addr;
};



struct sock_addr_in {
	unsigned char	    sin_len;
	unsigned char	    sin_family;
	unsigned short	    sin_port;
	struct	in_addr_r   sin_addr;
	char    	        sin_zero[8];
};




#endif

