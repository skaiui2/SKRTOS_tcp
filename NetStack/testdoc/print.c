#include <stdio.h>
#include <sys/socket.h> // 包含协议簇相关宏定义
#include <netinet/in.h> // 包含网络协议相关定义
#include <arpa/inet.h>  // 包含地址转换工具

int main() {
    // 打印常用协议簇的名称及其值
    printf("Protocol Families:\n");
    printf("AF_UNSPEC      = %d\n", AF_UNSPEC);      // 未指定协议族
    printf("AF_INET        = %d\n", AF_INET);        // IPv4 协议族
    printf("AF_INET6       = %d\n", AF_INET6);       // IPv6 协议族
    printf("AF_UNIX        = %d\n", AF_UNIX);        // UNIX 本地套接字
    printf("AF_LOCAL       = %d\n", AF_LOCAL);       // 与 AF_UNIX 相同
    printf("AF_PACKET      = %d\n", AF_PACKET);      // 用于链路层数据包
    printf("AF_NETLINK     = %d\n", AF_NETLINK);     // 内核与用户空间通信
    printf("AF_BLUETOOTH   = %d\n", AF_BLUETOOTH);   // 蓝牙协议族

    return 0;
}

