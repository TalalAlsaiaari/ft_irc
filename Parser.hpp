#ifndef PARSER_HPP
#define PARSER_HPP


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
#include <iostream>
#include <map>
#include <exception>
#include "Client.hpp"

class Parser {
	private:
		std::string input;
		std::string cmd;
		std::vector<std::string> args;
		std::map<std::string, Client> nicks;
		std::string pass;
		// std::map<std::string, Channel> channels;
	public:
		Parser( );
		~Parser( );
		void takeInput( std::string input );
		void findCmd( );
		std::string getCmd( void );
		void addNick( std::string nick, Client &client );
		void findNick( std::string input, Client &client );
		// void findArgs( );
		// void printCmd( );
};

#endif