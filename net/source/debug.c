#include "debug.h"


void print_buf_content(struct buf *buf, int len) 
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


#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void print_ip(unsigned int ip) {
    static int counter = 0; 
    counter++;

    struct in_addr ip_addr;
    ip_addr.s_addr = htonl(ip);  

    printf("Call #%d - IP Address: %s | Line: %d | Function: %s\n",
           counter, inet_ntoa(ip_addr), __LINE__, __func__);
}

