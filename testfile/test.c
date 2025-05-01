
#include "tapif.h"
#include "mempool.h"
#include "arp.h"
#include "ether.h"
#include "ifnet.h"
#include "debug.h"
#include "ip.h"
#include "tcp.h"
#include <pthread.h>


void net_init()
{
    pool_buf_handle = mempool_creat(sizeof(struct buf), 20);
    eth_init();
    arp_init();
    ip_init();
    tcp_init();
    socket_init();

    ifnet_init("192.168.1.200", "9e:4d:9e:e3:48:9f", 150);
}


void *net_thread(void *arg) 
{
    fd_set readfds;
    int ret;

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);

        ret = select(fd + 1, &readfds, NULL, NULL, NULL);
        if (ret == -1) {
            perror("select error");
            break;
        } else if (ret > 0 && FD_ISSET(fd, &readfds)) {
            ether_input();
        }
    }
}


#define SERVER_IP "192.168.1.200"  
#define SERVER_PORT 1234        
#define BUFFER_SIZE 1024
void *udp_thread(void *arg) 
{
    struct _sockaddr_in serv_addr, client;
    char buffer[BUFFER_SIZE];
    struct inpcb *inp;

    inp = _socket(IPPROTO_UDP);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.addr = inet_addr("192.168.1.1"); 
    serv_addr.sin_port = htons(SERVER_PORT);

    _bind(inp, (struct _sockaddr *)&serv_addr);

    while (1) {
        int n = _recvfrom(buffer, inp, (struct _sockaddr *)&client);
        if (n < 0) {
            perror("Recvfrom error");
            continue;
        }
        buffer[n] = '\0'; 
        printf("Received : %s\n", buffer);

        strcpy(buffer, "hello world!");
        _sendto(buffer, 14, inp, (struct _sockaddr *)&client);
    }
   
}

/*
server

void *tcp_thread(void *arg) 
{
    struct inpcb *inp;
    inp = _socket(IPPROTO_TCP);

    struct sockaddr_in server_addr = {0}, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    _bind(inp, (struct sockaddr *)&server_addr);
    _listen(inp, 5);

    while (1) {
        int client_fd = _accept(inp, NULL, NULL);
        char buffer[100] = {0};
        read(client_fd, buffer, 100);
        
        strcpy(buffer, "hello world!");
        _sendto(buffer, 14, inp, (struct _sockaddr *)&client_addr);
    }
    
}
*/

// TCP Client
void *tcp_thread(void *arg) 
{
    struct inpcb *inp;
    inp = _socket(IPPROTO_TCP);

    struct sockaddr_in server_addr = {0}, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.1"); 
    server_addr.sin_port = htons(8080); 


    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr("192.168.1.200"); 
    client_addr.sin_port = htons(1234); 

    _bind(inp, (struct _sockaddr *)&client_addr);
    _connect(inp, (struct _sockaddr *)&server_addr);

    char buffer[20] = {0};
    strcpy(buffer, "Hello, linux Server!");
    _sendto(buffer, sizeof(buffer), inp, (struct _sockaddr *)&server_addr);

    printf("Sending ACK to client\n");
    _shutdown(inp, SHUT_WR); 

    _close(inp);  
    while(1) {
        //_recvfrom(buffer, inp, (struct _sockaddr *)&client_addr);
        //printf("Received from server: %s\n", buffer);
    }
}




int main() {
    pthread_t thread_id1, thread_id2, thread_id3;

    fd = tapif_init();
    net_init();

    if (pthread_create(&thread_id1, NULL, net_thread, NULL) != 0) {
        perror("Failed to create thread1");
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread_id2, NULL, udp_thread, NULL) != 0) {
        perror("Failed to create thread2");
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread_id3, NULL, tcp_thread, NULL) != 0) {
        perror("Failed to create thread3");
        return EXIT_FAILURE;
    }
    

    while(1) {
        
    }
}


