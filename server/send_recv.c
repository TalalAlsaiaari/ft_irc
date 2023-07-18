#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	ft_putstr_fd(char *, int);
void	ft_putnbr_fd(int, int);
int main()
{
	struct addrinfo	hints, *res;
	int	sockfd;
	int	yes = 1;
	struct sockaddr_storage	their_addr; // Can use sockaddr_in??
	socklen_t	addr_size;
	int	new_fd;
	char	*serv_msg = "Hello from server!";
	int	len, bytes_sent, bytes_recvd;
	char	*buf;

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
	len = strlen(serv_msg);
	ft_putstr_fd("number of bytes supposed to send: ", 2);
	ft_putnbr_fd(len, 2);
	ft_putstr_fd("\n", 2);
	if ((bytes_sent = send(new_fd, serv_msg, len, 0)) < 0)
	{
		printf("send: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	ft_putstr_fd("number of bytes actually sent: ", 2);
	ft_putnbr_fd(bytes_sent, 2);
	ft_putstr_fd("\n", 2);
	while (1)
	{
		buf = (char *)malloc(1000);
		if ((bytes_recvd = recv(new_fd, buf, 1000, 0)) < 0)
		{
			printf("recv: %s. Exiting\n", strerror(errno));
			exit(1);
		}
		if (!bytes_recvd)
		{
			send(new_fd, "bye", strlen("bye"), 0);
			free(buf);
			break ;
		}
		else
			ft_putstr_fd(buf, 1);
		free(buf);
	}
	return 0;
}

/*Discrepancies between wanted bytes to send and actual bytes sent can happen when the size of the packet
is greater than 1K (probably). So it's better to find a way to avoid that*/