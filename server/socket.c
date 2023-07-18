#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	// int s;
	// struct addrinfo hints, *res;
	// // do the lookup
	// // [pretend we already filled out the "hints" struct]
	// getaddrinfo("www.example.com", "http", &hints, &res);
	// // again, you should do error-checking on getaddrinfo(), and walk
	// // the "res" linked list looking for valid entries instead of just
	// // assuming the first one is good (like many of these examples do).
	// // See the section on client/server for real examples.

	// s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	int	socket_fd;

	if ((socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		printf("Failed socket: %s\n. Exiting!", strerror(errno));
		exit(1);
	}
	printf("Sock fd: %d\n", socket_fd);
	return 0;
}