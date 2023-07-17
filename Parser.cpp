#include "Parser.hpp"

Parser::Parser() {
	func[std::string("NICK")] = &Parser::NICK;
}

Parser::~Parser() {}

void Parser::takeInput( std::string Input, int fd, Client client ) {
	this->input = Input;
	this->fd = fd;
	this->findCmd();
	try {
		clients.at(fd);
	} catch (std::exception &e) {
		clients[fd] = client;
	}
	try {
		func.at(cmd);
		((*this).*(func[cmd]))();
	} catch (std::exception &e) {
		// cmd not found
	}
}

void Parser::findCmd( ) {
	std::string::size_type pos2 = input.find_first_of(" ");
	cmd = input.substr(0, pos2);
	std::cout << "|" << cmd << "|" << std::endl;
}

std::string Parser::getCmd( void ) {
	return cmd;
}
