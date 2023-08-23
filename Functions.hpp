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
		std::map<std::string, Client *> nicks;
		Client *current_client;
		std::string pass;
		std::string const serverOpPass;
		std::map<std::string, Channel> channels;
		std::map<std::string, Client *> sent;
		typedef std::map<std::string, Channel>::iterator chan_it;
		typedef std::map<std::string, Client *>::iterator client_it;

	public:
		Functions( );
		virtual ~Functions( );

		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Setters ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		void setPass( std::string pass );

		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getters ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		std::string getPass( void ) const;

		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Booleans ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		bool isEnoughParams(unsigned int paramNum);
		bool channelExist(std::string chanName, chan_it channel);
		bool userInChan(std::string chanName, chan_it channel);
		bool isChanName( std::string name );
		bool checkNick( std::string nick );
		bool RegisterUser( void );

		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		void userMode(std::string modes);
		void channelMode(std::string modes, chan_it chan);
		void updateChannel(Client &client, std::string old_nick, std::string new_nick);
		std::vector<std::string> split(std::string str, std::string delim);
		void killMsg(Client &source, Client &dest);
		void quitMsg(Client &source, std::string msg);
		void errMsg(client_it dest, std::string msg);
		
		virtual void takeInput( std::string input, int fd, Client &client ) = 0;
};
