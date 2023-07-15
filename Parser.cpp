#include "Parser.hpp"

Parser::Parser() {}

Parser::~Parser() {}

void Parser::takeInput( std::string Input ) {
	input = Input;
}

void Parser::findCmd( ) {
	std::string::size_type pos2 = input.find_first_of(" ");
	cmd = input.substr(0, pos2);
}

// void Parser::findArgs( ) {

// }

void Parser::printCmd( ) {
	std::cout << "cmd: " << "|" << cmd << "|" << std::endl;
}