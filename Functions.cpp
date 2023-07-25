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
	if (nick.find_first_of("#&\x03") != nick.npos)
		ServerMessage(ERR_ERRONEUSNICKNAME, ":" + nick + " erroneus nick name\n");
	else {
		try {
			nicks.at(nick);
			ServerMessage(ERR_NICKNAMEINUSE, nick + " :Nickname is already in use\n");
		} catch ( std::exception &e ) {
			UserMessage(" " + nick + " :" + nick + "\n");
			nicks.erase(clients[fd].getNick());
			clients[fd].setNick(nick);
			nicks[nick] = clients[fd];
		}
	}
}

void Functions::NICK( void ) {
	try {
		args.at(0);
		if (args[0].empty())
			ServerMessage(ERR_NONICKNAMEGIVEN, ":need to give a nick name\n");
		else
			this->addNick(args[0]);
	} catch (std::exception &e) {
		ServerMessage(ERR_NONICKNAMEGIVEN, ":need to give a nick name\n");
	}
}

void Functions::CAP( void ) {
	if (args[0] == "LS")
		send(fd, "CAP * LS :multi-prefix userhost-in-names\n", strlen("CAP * LS :multi-prefix userhost-in-names\n"), 0);
	if (args[0] == "REQ") {
		std::string mes = "CAP * ACK " + args[1] + "\n";
		send(fd, &mes[0], mes.length(), 0);
	}
		// send(fd, "CAP * ACK multi-prefix\n", strlen("CAP * ACK multi-prefix\n"), 0);
}

void Functions::JOIN( void ) {
	if (clients[fd].isRegistered()) {
		if (args[0] == ":")
			ServerMessage(ERR_NEEDMOREPARAMS, cmd + " :Not enough parameters\n");
		else
			ServerMessage(ERR_NOSUCHCHANNEL, args[0] + " :No such channel\n");
	} else
		ServerMessage(ERR_NOTREGISTERED, ":you must register first\n");
}

void Functions::RegisterUser( void ) {
	try {
		args.at(0);
		if (clients[fd].getUserName().empty())
			clients[fd].setUserName("~" + args[0]);
		args.pop_front();
		args.at(0);
		if (clients[fd].getHostName().empty())
			clients[fd].setHostName(args[0]);
		args.pop_front();
		args.at(0);
		clients[fd].setServerName(args[0]);
		args.pop_front();
		clients[fd].registration();
		ServerMessage(RPL_WELCOME, " :Welcome You are now known as " + USER_FN(clients[fd].getNick(), clients[fd].getUserName(), clients[fd].getHostName()) + "\n" );
		this->MOTD();
		args.at(0);
		clients[fd].setRealName(args[0]);
		args.pop_front();
	} catch (std::exception &e) {
		ServerMessage(ERR_NEEDMOREPARAMS, ":need more params\n");
	}
}

void Functions::USER( void ) {
	if (!clients[fd].isRegistered()) {
		if (clients[fd].isPassGood())
			this->RegisterUser();
	} else
		ServerMessage(ERR_ALREADYREGISTERED, ":you're already reistered\n");
}

void Functions::MODE( void ) {
	// check for user modes, have to check same for channels
	try {
		std::map<std::string, Client>::iterator target = nicks.find(args.at(0));
		if (target == nicks.end())
			ServerMessage(ERR_NOSUCHNICK, ":" + args[0] + "\n");
		else if (target->second.getFD() != fd)
			ServerMessage(ERR_USERSDONTMATCH, " :Can't touch this\n");
		else {
			try {
				args.at(1);
				ServerMessage(ERR_UMODEUNKOWNFLAG, ":Unknown MODE flag " + args[1] + "\n");
			} catch (std::exception &e) {
				ServerMessage(RPL_UMODEIS, target->second.getNick() + " :\n");
			}
		}
	} catch (std::exception &e) {
		ServerMessage(ERR_NEEDMOREPARAMS, ":" + cmd + " need more params\n");
	}
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
		// ServerMessage(ERR_NOSUCHCHANNEL, args[0] + "\n");
	} catch (std::exception &e) {
		ServerMessage(ERR_NEEDMOREPARAMS, " :need more params\n");
	}
}

void Functions::UsertoUser(Client origin, Client dest) {
	std::string message = ":" + USER_FN(origin.getNick(), origin.getUserName(), origin.getHostName());
	message += " " + cmd + " " + origin.getNick() + " ";
	message += args.front();
	std::cout << message << std::endl;
	send(dest.getFD(), &message[0], message.length(), 0);
}

void Functions::PRIVMSG( void ) {
	try {
		args.at(0);
		args.at(1);
		try {
			//need to send to channels
			nicks.at(args[0]);
			UsertoUser(clients[fd], nicks[args[0]]);
		} catch (std::exception &e) {
			ServerMessage(ERR_NOSUCHNICK, ":" + args[0] + "\n");
		}
	} catch ( std::exception &e ) {
		ServerMessage(ERR_NEEDMOREPARAMS, " :need more params\n");
	}
}

void Functions::PASS( void ) {
	if (!clients[fd].isPassGood()) {
		try {
			args.at(0);
			if (args[0] == pass)
				clients[fd].passGood();
			else
				ServerMessage(ERR_PASSWDMISMATCH, ":password doesn't match\n");
		} catch (std::exception &e) {
			ServerMessage(ERR_NEEDMOREPARAMS, "PASS :need more params\n");
		}
	} else
		ServerMessage(ERR_ALREADYREGISTERED, ":you're already registered\n");
}

void Functions::MOTD( void ) {
	std::fstream file;
	std::string tmp;

	file.open("ft_irc.motd");
	if (!file.is_open()) {
		ServerMessage(ERR_NOMOTD, ":no MOTD\n");
	} else {
		ServerMessage(RPL_MOTDSTART, ":Message of the Day\n");
		while (std::getline(file, tmp))
			ServerMessage(RPL_MOTD, tmp + "\n");
		ServerMessage(RPL_ENDOFMOTD, ":End of /MOTD command.\a\n");
	}
}

void Functions::QUIT( void ) {
	std::map<int, Client>::iterator cli_fd;
	std::map<std::string, Client>::iterator cli_nick;
	std::string nick = clients[fd].getNick();
	cli_fd = clients.find(fd);
	cli_nick = nicks.find(nick);

	if (cli_fd != clients.end())
		clients.erase(cli_fd);
	else
		std::cerr << "no fd found\n";
	if (cli_nick != nicks.end())
		nicks.erase(cli_nick);
	else
		std::cerr << "no nick found\n";
}

// PRIVMSG alexhmball :hey
// :alexandraballer!~user@5.195.225.158 PRIVMSG alexhmball :hey

// NICK aballer
//      user_name   host_name        server_name      :real_name
// USER    user      user        host.docker.internal :Unknown