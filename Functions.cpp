#include "Functions.hpp"


Functions::Functions( ) {

}

Functions::~Functions( ) {

}

void Functions::setPass( std::string pass ) {
	this->pass = pass;
}

std::string Functions::getPass( void ) const {
	return this->pass;
}

void Functions::ServerMessage(std::string error, std::string message) {
	std::string mes = ":" + clients[fd].getServerName() + error + clients[fd].getNick() + " " + message;
	send(fd, &mes[0], mes.length(), 0);
}

void Functions::UserMessage(std::string message) {
	std::string mes = ":" + USER_FN(clients[fd].getNick(), clients[fd].getUserName(), clients[fd].getHostName()) + " " + cmd + " " + message;
	send(fd, &mes[0], mes.length(), 0);
}

// @time=2023-07-18T17:33:56.858Z :aballz!~user@5.195.225.158 NICK :ballers
void Functions::addNick( std::string nick ) {
	for (int i = 0; i < 2 && args.size(); i++)
		args.pop_front();
	if (args.size() > 0) {
		cmd = args.front();
		args.pop_front();
		if (cmd == "USER")
			this->USER();
		std::cout << cmd << std::endl;
	} else if (nick.empty()) {
		ServerMessage(ERR_NEEDMOREPARAMS, ":need to give a nick name\n");
	}
	try {
		nicks.at(nick);
		ServerMessage(ERR_NICKNAMEINUSE, nick + " :Nickname is already in use\n");
	} catch ( std::exception &e ) {
		if (clients[fd].getNick().empty()) {
			clients[fd].setNick(nick);
			nicks[nick] = clients[fd];
			if (clients[fd].isRegistered())
				ServerMessage(RPL_WELCOME, " :Welcome You are now known as " + USER_FN(nick, clients[fd].getUserName(), clients[fd].getHostName()) + "\n" );
			else
				ServerMessage(ERR_NOTREGISTERED, ":You have not registered\n");
		} else {
			UserMessage(" " + nick + " :" + nick + "\n");
			clients[fd].setNick(nick);
			nicks[nick] = clients[fd];
		}
	}
}

void Functions::NICK( void ) {
	try {
		args.at(0);
		std::string nick = args[0];
		this->addNick(args[0]);
	} catch (std::exception &e) {
		std::cout << "no nick given\n";
	}
	// ServerMessage(ERR_NOTREGISTERED, ":You have not registered\n");
}

void Functions::CAP( void ) {
	std::string mes = "CAP * ACK " + args[1] + "\n";
	if (args[0] == "LS")
		send(fd, "CAP * LS :multi-prefix userhost-in-names\n", strlen("CAP * LS :multi-prefix userhost-in-names\n"), 0);
	if (args[0] == "REQ")
		send(fd, &mes[0], mes.length(), 0);
		// send(fd, "CAP * ACK multi-prefix\n", strlen("CAP * ACK multi-prefix\n"), 0);
}

void Functions::JOIN( void ) {
	if (args[0] == ":")
		ServerMessage(ERR_NEEDMOREPARAMS, cmd + " :Not enough parameters\n");
	else
		ServerMessage(ERR_NOSUCHCHANNEL, args[0] + " :No such channel\n");
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
		clients[fd].registration();
		args.at(0);
		clients[fd].setRealName(args[0]);
		args.pop_front();
	} catch (std::exception &e) {
		std::cout << "user registration problem" << std::endl;
	}
}

void Functions::MODE( void ) {

}

void Functions::PING( void ) {
	std::string pong = ":" + clients[fd].getServerName() + " PONG " + clients[fd].getServerName() + " :" + clients[fd].getServerName() + "\n";
	send(fd, &pong[0], pong.length(), 0);
}

void Functions::PART( void ) {
	try {
		//need to check for multiple channels in args
		args.at(0);
		//need to check user is in the channel / channel exists
		UserMessage(args[0] + "\n");
		ServerMessage(ERR_NOSUCHCHANNEL, args[0] + "\n");
	} catch (std::exception &e) {
		ServerMessage(ERR_NEEDMOREPARAMS, " :need more params\n");
	}
}

void Functions::UsertoUser(Client origin, Client dest) {
	std::string message = ":" + USER_FN(origin.getNick(), origin.getUserName(), origin.getHostName());
	message += " " + cmd + " " + origin.getNick() + " ";
	args.pop_front();
	for (size_t i = 0; i < args.size(); i++) {
		message += args[i] + " ";
	}
	// might not need this line depending on parsing
	message += "\n";
	std::cout << message << std::endl;
	send(dest.getFD(), &message[0], message.length(), 0);
}

void Functions::PRIVMSG( void ) {
	try {
		args.at(0);
		args.at(1);
		try {
			//need to send to multiple users / channels
			nicks.at(args[0]);
			UsertoUser(clients[fd], nicks[args[0]]);
		} catch (std::exception &e) {
			ServerMessage(ERR_NOSUCHNICK, args[0] + "\n");
		}
	} catch ( std::exception &e ) {
		ServerMessage(ERR_NEEDMOREPARAMS, " :need more params\n");
	}
}

void Functions::PASS( void ) {
	if (!clients[fd].isRegistered()) {
		try {
			args.at(0);
			if (args[0] == pass) {
				// clients[fd].registration();
				if (args.size() > 3) {
					args.pop_front();
					args.pop_front();
					cmd = args.front();
					args.pop_front();
					if (cmd == "NICK")
						this->NICK();
				}
			} else {
				ServerMessage(ERR_PASSWDMISMATCH, ":password doesn't match\n");
			}
		} catch (std::exception &e) {
			ServerMessage(ERR_NEEDMOREPARAMS, "PASS :need more params\n");
		}
	} else {
		ServerMessage(ERR_ALREADYREGISTERED, ":you're already registered\n");
	}
}

void Functions::MOTD( void ) {
	std::fstream file;
	std::string tmp;
	std::string message;
	std::string::size_type pos;

	file.open("ft_irc.motd");
	if (!file.is_open()) {
		ServerMessage(ERR_NOMOTD, ":no MOTD\n");
	} else {
		ServerMessage(RPL_MOTDSTART, ":Message of the Day\n");
		while (std::getline(file, tmp)) {
			pos = tmp.find_first_of("01", 0);
			while( pos != tmp.npos) {
				tmp.insert(pos, "\x03");
				if (tmp.find_first_of("██║") == tmp.npos)
					tmp.insert(pos, tmp.substr(pos + 1, 2));
				pos = tmp.find_first_of("01", pos + 4);
			}
			ServerMessage(RPL_MOTD, tmp + "\n");
		}
		ServerMessage(RPL_ENDOFMOTD, ":End of /MOTD command.\n");
		std::cout << message << std::endl;
	}
}

// PRIVMSG alexhmball :hey
// :alexandraballer!~user@5.195.225.158 PRIVMSG alexhmball :hey

// NICK aballer
//      user_name   host_name        server_name      :real_name
// USER    user      user        host.docker.internal :Unknown