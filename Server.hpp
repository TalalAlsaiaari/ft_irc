/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: talsaiaa <talsaiaa@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 18:43:40 by talsaiaa          #+#    #+#             */
/*   Updated: 2023/07/28 09:59:09 by talsaiaa         ###   ########.fr       */
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
	typedef std::vector<Channel *>::iterator    channel_iterator;
	
	private:
		int				sockfd;
		int				new_client;
		int				fd_size;
		int				fd_count;
		int				nbytes;
		int				sender_fd;
		std::string		port;
		struct addrinfo	*res;
		
		// // Channels that are created on the server
		// std::map<std::string, Channel *>  channels;

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
		
		// // Method to get the channel from this server
		// const std::map<std::string, Channel*>&	getAllChannels();

		Server();
		~Server();
};
