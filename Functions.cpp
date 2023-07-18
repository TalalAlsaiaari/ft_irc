#include "Functions.hpp"

Functions::Functions( ) {

}

Functions::~Functions( ) {

}

Functions::ErrorMessage(std::string error, std::string message) {
	std::string mes = ":" + clients[fd].getServerName() + error + clients[fd].getNick() + " " + message;
	send(fd, &mes[0], mes.length(), 0);
}

// @time=2023-07-18T17:33:56.858Z :aballz!~user@5.195.225.158 NICK :ballers
void Functions::addNick( std::string nick ) {
	for (int i = 0; i < 2 && args.size(); i++)
		args.pop_front();
	// args.pop_front();
	if (args.size() > 0) {
		cmd = args.front();
		args.pop_front();
		if (cmd == "USER")
			this->USER();
		std::cout << cmd << std::endl;
	}
	try {
		nicks.at(nick);
		ErrorMessage(ERR_NICKNAMEINUSE, nick + " :Nickname is already in use\n");
	} catch ( std::exception &e ) {
		std::string yes;
		if (clients[fd].getNick().empty()) {
			clients[fd].setNick(nick);
			nicks[nick] = clients[fd];
			ErrorMessage(RPL_WELCOME, " :Welcome You are now known as " + USER_FN(nick, clients[fd].getUserName(), clients[fd].getHostName()) + "\n");
		} else {
			yes = ":" + USER_FN(clients[fd].getNick(), clients[fd].getUserName(), clients[fd].getHostName()) + " " + cmd + " " + nick + " :" + nick + " \n";
			clients[fd].setNick(nick);
			nicks[nick] = clients[fd];
		}
		std::cout << yes;

	}
}

void Functions::NICK( void ) {
	try {
		args.at(0);
		this->addNick(args[0]);
		// args.pop_front();
		// args.pop_front();
	} catch (std::exception &e) {
		std::cout << "no nick given\n";
	}
}

void Functions::CAP( void ) {
	if (args[0] == "LS")
		send(fd, "CAP * LS :multi-prefix sasl=PLAIN,EXTERNAL server-time draft/packing=EX1,EX2\n\r", strlen("CAP * LS :multi-prefix sasl=PLAIN,EXTERNAL server-time draft/packing=EX1,EX2\n"), 0);
	if (args[0] == "REQ")
		send(fd, "CAP * ACK multi-prefix\n\r", strlen("CAP * ACK multi-prefix\n"), 0);
}

void Functions::JOIN( void ) {
	std::string mes = ":" + clients[fd].getServerName();
	if (args[0] == ":")
		mes += " 461 " + cmd + " :Not enough parameters\n";
	else
		mes += " 403 " + args[0] + " :No such channel\n";
	send(fd, &mes[0], mes.length(), 0);
	// send(fd, ":No such channel\n", strlen(":No such channel\n"), 0);
}

void Functions::USER( void ) {
	try {
		args.at(0);
		clients[fd].setUserName(args[0]);
		args.pop_front();
		args.at(0);
		clients[fd].setHostName(args[0]);
		args.pop_front();
		args.at(0);
		clients[fd].setServerName(args[0]);
		args.pop_front();
		clients[fd].setRealName(args[0]);
		args.pop_front();
	} catch (std::exception &e) {
		std::cout << "user registration problem" << std::endl;
	}
}

void Functions::MODE( void ) {

}

void Functions::PING( void ) {

}

// NICK aballer
//      user_name   host_name        server_name      :real_name
// USER    user      user        host.docker.internal :Unknown