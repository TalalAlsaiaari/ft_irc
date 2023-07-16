#include "Parser.hpp"

Parser::Parser() {}

Parser::~Parser() {}

void Parser::takeInput( std::string Input ) {
	input = Input;
	this->findCmd();
}

void Parser::findCmd( ) {
	std::string::size_type pos2 = input.find_first_of(" ");
	cmd = input.substr(0, pos2);
}

std::string Parser::getCmd( void ) {
	return cmd;
}

void Parser::addNick(std::string nick, Client &client) {
	try {
		std::string err = nick + " :Nickname is already in use\n";
		nicks.at(nick);
		send(client.getFD(), &err[0], err.length(), 0);
	} catch ( std::exception &e ) {
		std::string yes = " :You are now known as " + nick + "\n";
		nicks[nick] = client;
		client.setNick(nick);
		send(client.getFD(), &yes[0], yes.length(), 0);
	}
}

void Parser::findNick(std::string input, Client &client) {
	std::string nick = input.substr(input.find_first_of(" ") + 1, input.find_first_of("\r"));
	std::cout << "nick: " << nick << std::endl;
	this->addNick(nick, client);
}

// void Parser::findArgs( ) {

// }

// void Parser::printCmd( ) {
// 	std::cout << "cmd: " << "|" << cmd << "|" << std::endl;
// }