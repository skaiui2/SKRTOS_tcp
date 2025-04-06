#ifndef TAPIF_H
#define TAPIF_H
#include "error.h"
#include "buf.h"


int tapif_init();
struct buf_struct *tapif_input();
int tapif_output(struct buf_struct *sk, uint16_t len);
void tapif_thread();

extern int fd;

#endif

