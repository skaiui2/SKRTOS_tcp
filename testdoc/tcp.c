#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.1.200" // 服务器 IP 地址
#define SERVER_PORT 1234         // 服务器端口

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char send_buffer[1024] = "Hello from Linux TCP Client!";
    char recv_buffer[1024];

    // 创建 TCP 套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 配置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid server IP address");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 连接到服务器
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to server %s:%d\n", SERVER_IP, SERVER_PORT);

    // 发送数据到服务器
    if (send(sockfd, send_buffer, strlen(send_buffer), 0) < 0) {
        perror("Failed to send data");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Sent to server: %s\n", send_buffer);

    // 接收服务器数据
    int bytes_received = recv(sockfd, recv_buffer, sizeof(recv_buffer) - 1, 0);
    if (bytes_received < 0) {
        perror("Failed to receive data");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    recv_buffer[bytes_received] = '\0'; // 添加字符串结束符
    printf("Received from server: %s\n", recv_buffer);

    // 关闭套接字
    close(sockfd);
    printf("Connection closed.\n");

    return 0;
}


