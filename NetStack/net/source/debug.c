#include "debug.h"


void print_buf_content(struct buf_struct *buf, int len) 
{
    if (!buf) {
        fprintf(stderr, "Buffer structure is NULL.\n");
        return;
    }

    size_t bytes_to_print = len;

    printf("Printing %zu bytes from buffer:\n", bytes_to_print);

    for (size_t i = 0; i < bytes_to_print; i++) {
        printf("%02X ", (unsigned char)buf->data_buf[i]); 
        if ((i + 1) % 16 == 0) {
            printf("\n"); 
        }
    }

    if (bytes_to_print % 16 != 0) {
        printf("\n");
    }
}


void print_content(char *buf, uint16_t len) 
{
    if (!buf) {
        fprintf(stderr, "Buffer structure is NULL.\n");
        return;
    }

    size_t bytes_to_print = len;

    for (size_t i = 0; i < bytes_to_print; i++) {
        printf("%02X ", (unsigned char)buf[i]); 
        if ((i + 1) % 16 == 0) {
            printf("\n"); 
        }
    }

    if (bytes_to_print % 16 != 0) {
        printf("\n");
    }
}

void print_mac(unsigned char mac[]) {
    printf("%02X:%02X:%02X:%02X:%02X:%02X\r\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}