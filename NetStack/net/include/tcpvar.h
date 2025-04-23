#ifndef TCPVAR_H
#define TCPVAR_H
#include "macro.h"
#include "list.h"
#include "inpcb.h"


typedef	unsigned int	tcp_seq;

#define	TF_ACKNOW	0x0001		/* ack peer immediately */
#define	TF_DELACK	0x0002		/* ack, but try to delay it */
#define	TF_NODELAY	0x0004		/* don't delay packets to coalesce */
#define	TF_NOOPT	0x0008		/* don't use tcp options */
#define	TF_SENTFIN	0x0010		/* have sent FIN */
#define	TF_REQ_SCALE	0x0020		/* have/will request window scaling */
#define	TF_RCVD_SCALE	0x0040		/* other side has requested scaling */
#define	TF_REQ_TSTMP	0x0080		/* have/will request timestamps */
#define	TF_RCVD_TSTMP	0x0100		/* a timestamp was received in SYN */
#define	TF_SACK_PERMIT	0x0200		/* other side said I could SACK */


#define	TCPOOB_HAVEDATA	0x01
#define	TCPOOB_HADDATA	0x02

struct tcpcb {
    struct list_node *node;
	short	t_state;		/* state of this connection */
	short	t_timer[TCPT_NTIMERS];	/* tcp timers */
	short	t_rxtshift;		/* log(2) of rexmt exp. backoff */
	short	t_rxtcur;		/* current retransmit value */
	short	t_dupacks;		/* consecutive dup acks recd */
	unsigned short	t_maxseg;		/* maximum segment size */
	char	t_force;		/* 1 if forcing out a byte */
	unsigned short	t_flags;

	struct	tcpiphdr *t_template;	/* skeletal packet for transmit */
	struct	inpcb *t_inpcb;		/* back pointer to internet pcb */
/*
 * The following fields are used as in the protocol specification.
 * See RFC783, Dec. 1981, page 21.
 */
/* send sequence variables */
	tcp_seq	snd_una;		/* send unacknowledged */
	tcp_seq	snd_nxt;		/* send next */
	tcp_seq	snd_up;			/* send urgent pointer */
	tcp_seq	snd_wl1;		/* window update seg seq number */
	tcp_seq	snd_wl2;		/* window update seg ack number */
	tcp_seq	iss;			/* initial send sequence number */
	unsigned int	snd_wnd;		/* send window */
/* receive sequence variables */
	unsigned int	rcv_wnd;		/* receive window */
	tcp_seq	rcv_nxt;		/* receive next */
	tcp_seq	rcv_up;			/* receive urgent pointer */
	tcp_seq	irs;			/* initial receive sequence number */

/* receive variables */
	tcp_seq	rcv_adv;		/* advertised window */
/* retransmit variables */
	tcp_seq	snd_max;		/* highest sequence number sent *
/* congestion control (for slow start, source quench, retransmit after loss) */
	unsigned int	snd_cwnd;		/* congestion-controlled window */
	unsigned int	snd_ssthresh;		
/*
 * transmit timing stuff.  See below for scale of srtt and rttvar.
 * "Variance" is actually smoothed difference.
 */
	short	t_idle;			/* inactivity time */
	short	t_rtt;			/* round trip time */
	tcp_seq	t_rtseq;		/* sequence number being timed */
	short	t_srtt;			/* smoothed round-trip time */
	short	t_rttvar;		/* variance in round-trip time */
	unsigned short	t_rttmin;		/* minimum rtt allowed */
	unsigned long	max_sndwnd;		/* largest window peer has offered */

/* out-of-band data */
	char	t_oobflags;		/* have some */
	char	t_iobc;			/* input character */
	short	t_softerror;		/* possible error not yet reported */

/* RFC 1323 variables */
	unsigned char	snd_scale;		/* window scaling for send window */
	unsigned char	rcv_scale;		/* window scaling for recv window */
	unsigned char	request_r_scale;	/* pending window scaling */
	unsigned char	requested_s_scale;
	unsigned long	ts_recent;		/* timestamp echo data */
	unsigned long	ts_recent_age;		/* when last updated */
	tcp_seq	last_ack_sent;

	void	*t_tuba_pcb;		/* next level down pcb for TCP over z */
};





#endif

