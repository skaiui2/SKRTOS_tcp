#ifndef SOCKET_VAR_H
#define SOCKET_VAR_H

typedef  int    pid_t; 
struct socket {
	short	so_type;		/* generic type, see socket.h */
	short	so_state;		/* internal state flags SS_*, below */
	void	*so_pcb;			/* protocol control block */
	struct	protosw *so_proto;	/* protocol handle */
	struct	socket *so_head;	/* back pointer to accept socket */
	struct	socket *so_q0;		/* queue of partial connections */
	struct	socket *so_q;		/* queue of incoming connections */
	short	so_q0len;		/* partials on so_q0 */
	short	so_qlen;		/* number of connections on so_q */
	short	so_qlimit;		/* max number queued connections */
	short	so_timeo;		/* connection timeout */
	unsigned short	so_error;		/* error affecting connection */
	pid_t	so_pgid;		/* pgid for signals */

	struct	sockbuf {
		unsigned long	sb_cc;		/* actual chars in buffer */
		unsigned long	sb_hiwat;	/* max actual char count */
		unsigned long	sb_mbcnt;	/* chars of mbufs used */
		unsigned long	sb_mbmax;	/* max chars of bufs to use */
		long	sb_lowat;	/* low water mark */
		struct	buf *sb_b;	/* the buf chain */
		short	sb_flags;	/* flags, see below */
		short	sb_timeo;	/* timeout for read/write */
	} so_rcv, so_snd;

};

#define	sbspace(sb) \
    ((long) imin((int)((sb)->sb_hiwat - (sb)->sb_cc), \
	 (int)((sb)->sb_mbmax - (sb)->sb_mbcnt)))



#endif

