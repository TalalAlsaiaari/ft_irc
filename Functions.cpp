#include "Functions.hpp"

Functions::Functions( ) {

}

Functions::~Functions( ) {

}

void Functions::addNick( std::string nick ) {
	try {
		std::string err = ":localhost 433 " + clients[fd].getNick() + " " + nick + " :Nickname is already in use\r\n";
		nicks.at(nick);
		send(fd, &err[0], err.length(), 0);
	} catch ( std::exception &e ) {
		std::string yes = ":localhost 001 :Welcome You are now known as " + nick + "!user@host\r\n";
		nicks[nick] = clients[fd];
		clients[fd].setNick(nick);
		send(fd, &yes[0], yes.length(), 0);
	}
}

void Functions::findNick( void ) {
	std::string nick = input.substr(input.find_first_of(' ') + 1, input.find_first_of('\r') - (input.find_first_of(' ') + 1));
	this->addNick(nick);
}

void Functions::NICK( void ) {
	this->findNick();
}

void Functions::CAP( void ) {

}
void Functions::JOIN( void ) {

}

// NICK aballer
// USER user user host.docker.internal :Unknown