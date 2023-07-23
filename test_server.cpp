#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <signal.h>
#include "Parser.hpp"
#include "Client.hpp"
#include <map>



struct addrinfo	*ft_getaddrinfo(std::string port)
{
	struct addrinfo hints;
	struct addrinfo	*res;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_TCP;
	if ((getaddrinfo(NULL, &port[0], &hints, &res)) < 0)
	{
		printf("getaddrinfo: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	return (res);
}

int	ft_socket(struct addrinfo *res)
{
	int	sockfd;

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0)
	{
		printf("socket: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	// freeaddrinfo(res);
	return (sockfd);
}

void	ft_setsockopt(int sockfd)
{
	int	yes;

	yes = 1;
	if ((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))) < 0)
	{
		printf("setsockopt: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	return ;
}

void	ft_bind(int sockfd, struct addrinfo *res)
{
	if ((bind(sockfd, res->ai_addr, res->ai_addrlen)) < 0)
	{
		printf("bind: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	freeaddrinfo(res);
	return ;
}

void	ft_listen(int sockfd)
{
	if ((listen(sockfd, 10)) < 0)
	{
		printf("listen: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	return ;
}

int	ft_new_client(int sockfd)
{
	struct sockaddr_storage	new_client_addr;
	socklen_t				new_client_addr_size;
	int						new_client;

	new_client_addr_size = sizeof(new_client_addr);
	new_client = accept(sockfd, (struct sockaddr *)&new_client_addr,
		&new_client_addr_size);
	if (new_client < 0)
	{
		printf("accept: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	return (new_client);
}

void	add_to_pfds(struct pollfd **pfds, int new_client, int *fd_count, int *fd_size)
{
	if (*fd_count == *fd_size)
	{
		*fd_size += 1;
		*pfds = (struct pollfd *)realloc(*pfds, sizeof(**pfds) * (*fd_size));
		if (!*pfds)
		{
			printf("realloc: %s. Exiting\n", strerror(errno));
			exit(1);
		}
	}
	(*pfds)[*fd_count].fd = new_client;
	(*pfds)[*fd_count].events = POLLIN;
	*fd_count += 1;
	return ;
}

void	del_from_pfds(struct pollfd *pfds, int i, int *fd_count)
{
	pfds[i] = pfds[*fd_count - 1];
	(*fd_count)--;
	return ;
}

int	main(int ac, char **av)
{
	struct addrinfo	*res;
	int				sockfd;
	int				new_client;
	struct pollfd	*pfds;
	int				fd_size;
	int				fd_count;
	int				poll_count;
	int				nbytes;
	int				sender_fd;
	char			buf[256];
	// int				dest_fd;
    std::map<int, Client> clients;
    Parser parser;
    std::string port;

    if (ac == 3) {
        port = av[1];
        parser.setPass(av[2]);
    } else {
        std::cout << "expected params: <port> <password>\n";
        return 1;
    }
	res = ft_getaddrinfo(port);
	sockfd = ft_socket(res);
	ft_setsockopt(sockfd);
	ft_bind(sockfd, res);
	ft_listen(sockfd);
	fd_size = 5;
	pfds = (struct pollfd *)malloc(fd_size * sizeof(pfds));
	pfds[0].fd = sockfd;
	pfds[0].events = POLLIN;
	fd_count = 1;
	while (1)
	{
		poll_count = poll(pfds, fd_count, -1);
		if (poll_count == -1)
		{
			printf("poll: %s. Exiting\n", strerror(errno));
			exit(1);
		}
		for (int i = 0; i < fd_count; i++)
		{
			if (pfds[i].revents & POLLIN)
			{
				if (pfds[i].fd == sockfd)
				{
					new_client = ft_new_client(sockfd);
                    clients[new_client] = Client(new_client);
					add_to_pfds(&pfds, new_client, &fd_count, &fd_size);
				}
				else
				{
					nbytes = recv(pfds[i].fd, buf, sizeof(buf), 0);
					sender_fd = pfds[i].fd;
					if (nbytes <= 0)
					{
						if (nbytes == 0)
							printf("clinet %d hung up\n", sender_fd);
						else
							perror("recv");
						close(pfds[i].fd);
						del_from_pfds(pfds, i, &fd_count);
					}
					else
					{
                        parser.takeInput(buf, sender_fd, clients[sender_fd]);
						memset(buf, 0, strlen(buf));
					}
				}
			}
		}
	}
	return 0;
}
