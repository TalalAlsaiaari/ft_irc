#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <iostream>

class Parser {
	private:
		std::string input;
		std::string cmd;
		std::vector<std::string> args;
	public:
		Parser( );
		~Parser( );
		void takeInput( std::string input );
		void findCmd( );
		// void findArgs( );
		void printCmd( );
};

#endif