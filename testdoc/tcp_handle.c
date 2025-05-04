#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFSIZE 1024

void handle_client(int client_fd) {
    char buffer[BUFSIZE];
    ssize_t bytes_read;

    // Step 1: Read client data
    bytes_read = recv(client_fd, buffer, BUFSIZE - 1, 0);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("Received: %s\n", buffer);

        // Step 2: Respond once
        send(client_fd, "Hello, client!", 14, 0);

        // Step 3: Wait for FIN from client
        bytes_read = recv(client_fd, buffer, BUFSIZE - 1, 0);
        if (bytes_read == 0) {
            printf("Client sent FIN.\n");
        } else if (bytes_read < 0) {
            perror("recv error after response");
        }
    } else if (bytes_read == 0) {
        printf("Client closed without sending data.\n");
    } else {
        perror("recv failed initially");
    }

    // Step 4: Send FIN
    shutdown(client_fd, SHUT_RDWR);  // triggers FIN
    close(client_fd);                // ensures FIN gets out
    printf("Closed connection properly (FIN sent).\n");
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept + handle
    while (1) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        printf("Connection from %s:%d\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        handle_client(client_fd);
    }

    return 0;
}

