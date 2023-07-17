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
#include <iostream>
#include <map>
#include <exception>
#include <sstream>
#include "Client.hpp"
#include "ErrorCodes.hpp"

template < typename T >
void StrtoAny(std::string str, T &value) {
	std::stringstream ss(str);

	ss >> value;
}

template < typename T >
std::string AnytoStr(T value) {
	std::stringstream ss;

	ss << value;
	return ss.str();
}

class Functions {
	protected:
		int fd;
		std::string input;
		std::string cmd;
		std::vector<std::string> args;
		std::map<std::string, Client> nicks;
		std::map<int, Client> clients;
		std::string pass;
	public:
		Functions( );
		virtual ~Functions( );
		virtual void takeInput( std::string input, int fd, Client client ) = 0;
		void addNick( std::string nick );
		void findNick( void );
		void NICK( void );
};

