#ifndef IN_H
#define IN_H


struct _in_addr {
    unsigned int   addr;
};



struct _sockaddr_in {
	unsigned char	    sin_len;
	unsigned char	    sin_family;
	unsigned short	    sin_port;
	struct	_in_addr   	sin_addr;
	char    	        sin_zero[8];
};


unsigned short in_checksum(void *b, int len);




#endif

