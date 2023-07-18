// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
int main(int argc, char const* argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };

	// Creating socket file descriptor
	if ((server_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	// SO_REUSEADDR is used so that the TCP socket be available immediately after closing.
	// Though this makes TCP less reliable.
	address.sin_family = PF_INET;
	address.sin_addr.s_addr = INADDR_ANY; //socket will be bound to all local interfaces
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen))
		< 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	while (1) {
		valread = read(new_socket, buffer, 1024);
		printf("%s\n", buffer);
		// send(new_socket, hello, strlen(hello), 0);
		// printf("Hello message sent\n");
	}
	// closing the connected socket
	close(new_socket);
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
