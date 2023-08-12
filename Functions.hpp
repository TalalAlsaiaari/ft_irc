#pragma once

#include <netdb.h>
#include <string>
#include <vector>
#include <deque>
#include <iostream>
#include <map>
#include <exception>
#include <algorithm>
#include <sstream>
#include "Client.hpp"
#include "ErrorCodes.hpp"
#include "devector.hpp"
#include "IrcErrorException.hpp"
#include <fstream>
#include <unistd.h>
#include "Channel.hpp"

class Functions {
	protected:
		int fd;
		std::string input;
		std::string cmd;
		devector<devector<std::string> > multi_cmd;
		devector<std::string> args;
		std::map<std::string, Client> nicks;
		Client *current_client;
		std::string pass;
		std::string const operPass;
		std::map<std::string, Channel *> channels;
	public:
		Functions( );
		virtual ~Functions( );
		virtual void takeInput( std::string input, int fd, Client &client ) = 0;
		void setPass( std::string pass );
		std::string getPass( void ) const;
		void addNick( std::string nick );
		void ServerMessage(std::string error, std::string message);
		void UserMessage(std::string message);
		void UsertoUser(Client orgin, Client dest);
		void ConnectionMessage( void );
		void RegisterUser( void );
		void NICK( void );
		void CAP( void );
		void JOIN( void );
		void USER( void );
		void MODE( void );
		void PING( void );
		// void PART( void );
		void PRIVMSG( void );
		void NOTICE( void );
		void PASS( void );
		void MOTD( void );
		void QUIT( void );
		void WHOIS( void );
		void OPER(void);
		void KILL (void);
		void quitMsg(Client, std::string);
		void killMsg(Client, Client);
		void errMsg(std::map<std::string, Client>::iterator, std::string);
};
