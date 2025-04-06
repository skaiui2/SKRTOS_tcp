#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/if_tun.h>
#include <errno.h>
#include <linux/if.h>
#include <sys/select.h>
#include "tapif.h"


int fd;

int tapif_init()
{
    struct ifreq ifr;
    int err;

    if ((fd = open("/dev/net/tun", O_RDWR)) < 0) {
        SYS_ERROR("can't open/dev/net/tun");
        return fd;
    }
    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;

    strncpy(ifr.ifr_name, "tap0", IFNAMSIZ);

    if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0) {
        SYS_ERROR("ioctl(TUNSETIFF)");
        close(fd);
        return err;
    }

    return fd;
}

struct buf_struct *tapif_input()
{
    char buf[MLEN];
    struct buf_struct *sk;
    int readlen = read(fd, buf, MLEN);
    if (readlen < 0) {
        SYS_ERROR("read returned -1");
        return NULL;
    } 

    sk = buf_get();
    if (sk == NULL) {
        SYS_ERROR("can't allocate buf");
        return NULL;
    }
    buf_copy(sk, buf, MLEN);
    return sk;
}


int tapif_output(struct buf_struct *sk, uint16_t len)
{
  char buf[256]; 
  ssize_t written;

  buf_copy_to(buf, sk, len);

  /* signal that packet should be sent(); */
  written = write(fd, buf, len);
  if (written < 0) {
    SYS_ERROR("write returned -1");
    return written;
  }
  return true;
}


void tapif_thread()
{
  struct netif *netif;
  struct tapif *tapif;
  int ret;
  fd_set readfds;     

  while(1) {
    FD_ZERO(&readfds);        
    FD_SET(fd, &readfds);

    /* Wait for a packet to arrive. */
    ret = select(fd + 1, &readfds, NULL, NULL, NULL);
    if(ret == 1) {
      /* Handle incoming packet. */
      tapif_input(netif);
    } else if(ret == -1) {
      SYS_ERROR("tapif_thread: select");
    }
  }

}
