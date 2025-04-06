#include "debug.h"


void print_buf_content(struct buf_struct *buf) 
{
    if (!buf) {
        fprintf(stderr, "Buffer structure is NULL.\n");
        return;
    }

    size_t bytes_to_print = buf->len < 128 ? buf->len : 128;

    printf("Printing %zu bytes from buffer:\n", bytes_to_print);

    for (size_t i = 0; i < bytes_to_print; i++) {
        printf("%02X ", (unsigned char)buf->data[i]); 
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

