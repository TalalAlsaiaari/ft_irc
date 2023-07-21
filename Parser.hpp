#pragma once

#include "Functions.hpp"

// #define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

class Parser : public Functions {
	public:
		Parser( );
		~Parser( );
		void takeInput( std::string input, int fd, Client client );
		void findCmd( );
		std::string getCmd( void );
	private:
		typedef void (Parser::*Funcs)(void);
		std::map<std::string, Funcs> func;
};