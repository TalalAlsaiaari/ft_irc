/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ballzball <ballzball@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 19:20:51 by talsaiaa          #+#    #+#             */
/*   Updated: 2023/07/27 18:13:02 by ballzball        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

Server::Server()
{
	this->sockfd = -1;
	this->new_client = -1;
	this->fd_size = 5;
	this->fd_count = 1;
	this->nbytes = 0;
	this->sender_fd = -1;
	this->port = "6667";
	this->pfds = new struct pollfd[this->fd_size];
}

void	Server::setPort(char *port)
{
	this->port = port;
	return ;
}

void	Server::getAddrInfo(void)
{
	struct addrinfo	hints;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_TCP;
	if ((getaddrinfo(NULL, &this->port[0], &hints, &this->res)) < 0)
	{
		std::cout << "Error with getaddrinfo" << std::endl;
		exit(1);
	}
	return ;
}

void	Server::ftSocket(void)
{
	this->sockfd = socket(this->res->ai_family, this->res->ai_socktype,
		this->res->ai_protocol);
	if (this->sockfd < 0)
	{
		std::cout << "Error with socket" << std::endl;
		exit(1);
	}
	return ;
}

void	Server::ftSetSockOpt(void)
{
	int	yes = 1;

	if ((setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)))
		< 0)
	{
		std::cout << "Error with setsockopt" << std::endl;
		exit(1);
	}
	return ;
}

void	Server::ftBind(void)
{
	if ((bind(this->sockfd, this->res->ai_addr, this->res->ai_addrlen)) < 0)
	{
		std::cout << "Error with bind" << std::endl;
		exit(1);
	}
	return ;
}

void	Server::ftListen(void)
{
	if ((listen(this->sockfd, 10)) < 0)
	{
		std::cout << "Error with listen" << std::endl;
		exit(1);
	}
	return ;
}

void	Server::ftPoll(void)
{
	this->poll_count = poll(this->pfds, this->fd_count, -1);
	if (this->poll_count < 0)
	{
		std::cout << "Error with poll" << std::endl;
		exit(1);
	}
	return ;
}

void	Server::addNewClient(int index)
{
	struct sockaddr_in	new_client_addr;
	socklen_t			new_client_addr_size;

	new_client_addr_size = sizeof(new_client_addr);
	if (this->pfds[index].fd == this->sockfd)
	{
		this->new_client = accept(this->sockfd, (struct sockaddr *)&new_client_addr,
			&new_client_addr_size);
		if (this->new_client < 0)
		{
			std::cout << "Error with accept" << std::endl;
			exit(1);
		}
		std::cout << "Client IP: " << inet_ntoa(new_client_addr.sin_addr) << std::endl;
		this->clients[this->new_client] = Client(this->new_client, inet_ntoa(new_client_addr.sin_addr));
		this->resizePfds();
	}
	return ;
}

void	Server::resizePfds(void)
{
	struct pollfd	*temp;
	if (this->fd_count == this->fd_size)
	{
		this->fd_size++;
		temp = new struct pollfd[this->fd_size];
		for (int i = 0; i < this->fd_size - 1; i++)
		{
			temp[i].fd = this->pfds[i].fd;
			temp[i].events = this->pfds[i].events;
			temp[i].revents = this->pfds[i].revents;
		}
		delete this->pfds;
		this->pfds = temp;
	}
	this->pfds[this->fd_count].fd = this->new_client;
	this->pfds[this->fd_count].events = POLLIN;
	this->fd_count += 1;
	return ;
}

void	Server::ftRecv(int index)
{
	if (this->pfds[index].fd != this->sockfd)
	{
		this->nbytes = recv(this->pfds[index].fd, this->buf, sizeof(this->buf), 0);
		this->sender_fd = this->pfds[index].fd;
	}
	return ;
}

void	Server::checkNBytes(int index)
{
	if (this->nbytes < 0 && index != 0)
		std::cout << "Error with recv" << std::endl;
	if (!this->nbytes && index != 0)
		std::cout << "Client " << this->sender_fd << " disconnected" << std::endl;
	this->removeClient(index);
	if (this->nbytes > 0 && index != 0)
		this->ftSend();
	return ;
}

void	Server::removeClient(int index)
{
	if (this->nbytes <= 0 && index != 0)
	{
		close(this->pfds[index].fd);
		this->pfds[index] = this->pfds[this->fd_count - 1];
		this->fd_count--;
	}
	return ;
}

void	Server::ftSend(void)
{
	std::cout << this->buf << std::endl;
	this->parser.takeInput(buf, this->sender_fd, this->clients[this->sender_fd]);
	std::memset(this->buf, 0, 256);
	return ;
}

void	Server::ftIRC(void)
{
	this->getAddrInfo();
	this->ftSocket();
	this->ftSetSockOpt();
	this->ftBind();
	this->ftListen();
	this->pfds[0].fd = this->sockfd;
	this->pfds[0].events = POLLIN;
	while(true)
	{
		this->ftPoll();
		for (int i = 0; i < this->fd_count; i++)
		{
			if (this->pfds[i].revents & POLLIN)
			{
				this->addNewClient(i);
				this->ftRecv(i);
				this->checkNBytes(i);
			}
		}
	}
}

Server::~Server()
{
	delete this->pfds;
}