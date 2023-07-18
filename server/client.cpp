// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <iomanip>
#include <string>
#define PORT 8080

int main(int argc, char const* argv[])
{
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	char *buff;
	// char buffer[1024] = { 0 };
	int	rd;
	if ((client_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = PF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(PF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	while (1) {
		buff = (char *)calloc(1000, sizeof(char));
		rd = read(0, buff, strlen(buff));
		if (rd <= 0)
			break ;
		send(client_fd, buff, strlen(buff), 0);
		free(buff);
		// valread = read(client_fd, buffer, 1024);
		// printf("%s\n", buffer);
	}

	// closing the connected socket
	close(client_fd);
	return 0;
}
