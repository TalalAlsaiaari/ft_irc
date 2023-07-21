#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <vector>
#include <deque>
#include <iostream>
#include <map>
#include <exception>
#include <sstream>
#include "Client.hpp"
#include "ErrorCodes.hpp"
#include "devector.hpp"
#include <fstream>
// #include "Channel.hpp"
// #define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))
#define USER_FN(nick,user,host) (nick + "!" + user + "@" + host)
#define CTRL_C "\x03"
#define CTRL_V "\x16"
#define GREEN "\x03" + "3"
#define PURPLE CTRL_C + "13"
#define RED CTRL_C + "4"
#define YELLOW CTRL_C + "8"

class Functions {
	protected:
		int fd;
		std::string input;
		std::string cmd;
		devector<std::string> args;
		std::map<std::string, Client> nicks;
		// std::map<std::string, Channel> channels;
		std::map<int, Client> clients;
		std::string trailing;
		std::string pass;
	public:
		Functions( );
		virtual ~Functions( );
		virtual void takeInput( std::string input, int fd, Client client ) = 0;
		void setPass( std::string pass );
		std::string getPass( void ) const;
		void addNick( std::string nick );
		void ServerMessage(std::string error, std::string message);
		void UserMessage(std::string message);
		void UsertoUser(Client orgin, Client dest);
		void NICK( void );
		void CAP( void );
		void JOIN( void );
		void USER( void );
		void MODE( void );
		void PING( void );
		void PART( void );
		void PRIVMSG( void );
		void PASS( void );
		void MOTD( void );
};

