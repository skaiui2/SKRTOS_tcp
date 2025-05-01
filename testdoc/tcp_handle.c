#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080       // Server port number
#define BUFSIZE 1024    // Buffer size for incoming and outgoing data

void handle_client(int client_fd) {
    char buffer[BUFSIZE] = {0};

    // Receive application data from the client
    int bytes_received = recv(client_fd, buffer, BUFSIZE, 0);
    if (bytes_received > 0) {
        printf("Received data: %s\n", buffer);
    } else {
        printf("No data received or client closed connection.\n");
    }

    // Send a reply to the client
    const char *response = "Hello, client!";
    send(client_fd, response, strlen(response), 0);
    printf("Sent response to client.\n");

    // Close the client socket
    close(client_fd);
    printf("Closed connection with client.\n");
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // 1. Create a TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        return 1;
    }

    // 2. Set server address parameters
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Listen on all interfaces
    server_addr.sin_port = htons(PORT);

    // 3. Bind the socket to IP and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        return 1;
    }

    // 4. Start listening for incoming connections
    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        close(server_fd);
        return 1;
    }

    printf("Server is listening on port %d...\n", PORT);

    // 5. Main loop: accept and handle clients
    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        printf("Client connected: %s:%d\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        // Handle client communication
        handle_client(client_fd);
    }

    // 6. Close server socket (this line actually never reached because of infinite loop)
    close(server_fd);

    return 0;
}

