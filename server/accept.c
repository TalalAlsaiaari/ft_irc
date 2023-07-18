#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	struct addrinfo	hints, *res;
	int	sockfd;
	int	yes = 1;
	struct sockaddr_storage	their_addr; // Can use sockaddr_in??
	socklen_t	addr_size;
	int	new_fd;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_TCP;
	if (getaddrinfo(NULL, "6667", &hints, &res) < 0)
	{
		printf("getaddrinfo: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
	{
		printf("socket: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	/*the below is used to get rid of the Address already in use error for bind. That happens when a
	socket that was connected is still hanging around in the kernel hoggign the port*/
	if ((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))) < 0)
	{
		printf("setsockopt: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	if ((bind(sockfd, res->ai_addr, res->ai_addrlen)) < 0)
	{
		printf("bind: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	if ((listen(sockfd, 10)) < 0)
	{
		printf("listen: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	addr_size = sizeof(their_addr);
	/*accept will return a new_fd specified for that connection which can be used for communicating. listen
	will still use the old fd to listen for other connections*/
	if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size)) < 0)
	{
		printf("accept: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	return 0;
}