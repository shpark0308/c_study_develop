#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const int PORT = 8088;


int main() {
    int client_socket;
    sockaddr_in server_addr;

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];

    for (int i=0; i<10; i++) {
		snprintf(buffer, sizeof(buffer), "send to server : (%d)\n", (i+1));
		printf("%s\n", buffer);
        // Send user input to the server
        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
	return 0;
}
