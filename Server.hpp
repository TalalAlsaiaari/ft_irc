/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aball <aball@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 18:43:40 by talsaiaa          #+#    #+#             */
/*   Updated: 2023/07/27 00:29:09 by aball            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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
#include <string>
#include <cstring>
#include <iostream>
#include <map>
#include "Parser.hpp"

class	Client;

class	Server
{
	private:
		int				sockfd;
		int				new_client;
		int				fd_size;
		int				fd_count;
		int				nbytes;
		int				sender_fd;
		std::string		port;
		struct addrinfo	*res;

	public:
		char					buf[256];
		struct pollfd			*pfds;
		int						poll_count;
		std::map<int, Client>	clients;
		Parser					parser;
		void					setPort(char *);
		void					getAddrInfo(void);
		void					ftSocket(void);
		void					ftSetSockOpt(void);
		void					ftBind(void);
		void					ftListen(void);
		void					ftPoll(void);
		void					addNewClient(int);
		void					ftRecv(int);
		void					checkNBytes(int);
		void					ftSend(void);
		void					removeClient(int);
		void					resizePfds(void);
		void					ftIRC(void);

		Server();
		~Server();
};