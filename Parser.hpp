#pragma once

#include "Functions.hpp"

class Parser : public Functions {
	public:
		Parser( );
		~Parser( );
		void takeInput( std::string input, int fd, Client &client );
		void findCmdArgs( void );
		void findPass( void );
		void excecuteCommand( void );
		bool checkRegistration( void );
		std::string makeUpper( std::string str);
		void removeClient(std::string remove);
	private:
		typedef void (Parser::*Funcs)(void);
		std::map<std::string, Funcs> func;
};
