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
#include "Messages.hpp"

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
		std::map<std::string, Channel> channels;
		std::map<std::string, Client *> sent;
		typedef std::map<std::string, Channel>::iterator chan_it;
		typedef std::map<std::string, Client>::iterator client_it;
	public:
		Functions( );
		virtual ~Functions( );
		virtual void takeInput( std::string input, int fd, Client &client ) = 0;
		void setPass( std::string pass );
		void updateChannel(Client &client, std::string old_nick, std::string new_nick);
		std::string getPass( void ) const;
		bool checkNick( std::string nick );
		bool RegisterUser( void );
		void quitMsg(Client source, std::string msg);
		void killMsg(Client source, Client dest);
		void errMsg(std::map<std::string, Client>::iterator dest, std::string msg);
		std::vector<std::string> split(std::string str, std::string delim);
		
		bool isEnoughParams(unsigned int paramNum);
		bool channelExist(std::string chanName, chan_it channel);
		bool userInChan(std::string chanName, chan_it channel);
		bool isUserOp(std::string chanName);
		bool isChanName( std::string name );
		void userMode(std::string modes);
};
