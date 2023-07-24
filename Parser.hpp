#pragma once

#include "Functions.hpp"

class Parser : public Functions {
	public:
		Parser( );
		~Parser( );
		void takeInput( std::string input, int fd, Client client );
		void findCmdArgs( void );
	private:
		typedef void (Parser::*Funcs)(void);
		std::map<std::string, Funcs> func;
};
