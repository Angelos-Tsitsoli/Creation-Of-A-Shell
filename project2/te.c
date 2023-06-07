#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 256

int main() {
    int socket_fd;
    struct sockaddr_in server_addr;

    // Create socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("Socket creation failed");
        return -1;
    }

    // Set server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5634);
    server_addr.sin_addr.s_addr = inet_addr("localhost");

    // Connect to the server
    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(socket_fd);
        return -1;
    }

    // Read from the server
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    ssize_t bytes_read;
    while ((bytes_read = read(socket_fd, buffer, sizeof(buffer))) > 0) {
        printf("Received: %s\n", buffer);
        memset(buffer, 0, sizeof(buffer));
    }

    if (bytes_read == -1) {
        perror("Read failed");
    }

    // Close the socket
    close(socket_fd);

    return 0;
}