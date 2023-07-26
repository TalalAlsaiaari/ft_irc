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
	std::string mes = ":" + current_client->getServerName() + error + current_client->getNick() + " " + message;
	send(fd, &mes[0], mes.length(), 0);
}

void Functions::UserMessage(std::string message) {
	std::string mes = ":" + USER_FN(current_client->getNick(), current_client->getUserName(), current_client->getHostName()) + " " + cmd + " " + message;
	send(fd, &mes[0], mes.length(), 0);
}

// @time=2023-07-18T17:33:56.858Z :aballz!~user@5.195.225.158 NICK :ballers
void Functions::addNick( std::string nick ) {
	std::map<std::string, Client>::iterator it;

	it = nicks.find(nick);
	if (nick.find_first_of("#&\x03") != nick.npos)
		ServerMessage(ERR_ERRONEUSNICKNAME, ":" + nick + " erroneus nick name\n");
	else if (it != nicks.end()) {
		if (current_client->isRegistered())
			ServerMessage(ERR_NICKNAMEINUSE, nick + " :Nickname is already in use\n");
		else {
			ServerMessage("ERROR ", "13 your nick is unavailable, get a better name and try connecting again\n");
			multi_cmd.clear();
			this->QUIT();
			throw IrcErrorException("user tried to register with nick already in use\n");
		}
	} else {
		UserMessage(" " + nick + " :" + nick + "\n");
		nicks.erase(current_client->getNick());
		current_client->setNick(nick);
	}
}

void Functions::NICK( void ) {
	if (args.size() < 1)
		ServerMessage(ERR_NONICKNAMEGIVEN, ":need to give a nick name\n");
	else
		this->addNick(args[0]);
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
	if (current_client->isRegistered()) {
		if (args[0] == ":")
			ServerMessage(ERR_NEEDMOREPARAMS, cmd + " :Not enough parameters\n");
		else
			ServerMessage(ERR_NOSUCHCHANNEL, args[0] + " :No such channel\n");
	} else
		ServerMessage(ERR_NOTREGISTERED, ":you must register first\n");
}

void Functions::RegisterUser( void ) {
	Client *user = &clients[fd];
	try {
		if (user->getUserName().empty())
			user->setUserName("~" + args.at(0));
		if (user->getHostName().empty())
			user->setHostName(args.at(1));
		user->setServerName(args.at(2));
		user->registration();
		user->setRealName(args.at(3));
		if (user->getNick().empty())
			ServerMessage(ERR_NONICKNAMEGIVEN, ":no nick name given\n");
		else {
			ServerMessage(RPL_WELCOME, " :Welcome You are now known as " + USER_FN(user->getNick(), user->getUserName(), user->getHostName()) + "\n" );
			this->MOTD();
			nicks[user->getNick()] = *user;
		}
	} catch (std::exception &e) {
		ServerMessage(ERR_NEEDMOREPARAMS, ":need more params\n");
	}
}

void Functions::USER( void ) {
	if (!current_client->isRegistered()) {
		if (current_client->isPassGood())
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
	std::string pong = ":" + current_client->getServerName() + " PONG " + current_client->getServerName() + " :" + current_client->getServerName() + "\n";
	send(fd, &pong[0], pong.length(), 0);
}

void Functions::PART( void ) {
	try {
		//need to check user is in the channel / channel exists
		UserMessage(args.at(0) + "\n");
		// ServerMessage(ERR_NOSUCHCHANNEL, args[0] + "\n");
	} catch (std::exception &e) {
		ServerMessage(ERR_NEEDMOREPARAMS, " :need more params\n");
	}
}

void Functions::UsertoUser(Client origin, Client dest) {
	std::string message = ":" + USER_FN(origin.getNick(), origin.getUserName(), origin.getHostName());
	message += " " + cmd + " " + origin.getNick() + " ";
	args.pop_front();
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
			UsertoUser(clients[fd], nicks.at(args[0]));
		} catch (std::exception &e) {
			ServerMessage(ERR_NOSUCHNICK, ":" + args[0] + "\n");
		}
	} catch ( std::exception &e ) {
		ServerMessage(ERR_NEEDMOREPARAMS, " :need more params\n");
	}
}

void Functions::NOTICE( void ) {
	try {
		args.at(0);
		args.at(1);
		try {
			//need to send to channels
			UsertoUser(clients[fd], nicks.at(args[0]));
		} catch (std::exception &e) {
			// Don't send error back to user
			std::cout << "NOTICE to " + args[0] + " failed\n";
		}
	} catch ( std::exception &e ) {
		ServerMessage(ERR_NEEDMOREPARAMS, " :need more params\n");
	}
}

void Functions::PASS( void ) {
	if (!current_client->isPassGood()) {
		try {
			args.at(0);
			if (args[0] == pass)
				current_client->passGood();
			else {
				ServerMessage(ERR_PASSWDMISMATCH, "04 :password doesn't match\n");
				multi_cmd.clear();
			}
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
	std::string nick = current_client->getNick();
	cli_fd = clients.find(fd);
	cli_nick = nicks.find(nick);

	if (cli_fd != clients.end())
		clients.erase(cli_fd);
	if (cli_nick != nicks.end())
		nicks.erase(cli_nick);
	throw IrcErrorException("Client has quit\n");
}

void Functions::WHOIS( void ) {
	std::map<std::string, Client>::iterator user;
	std::string who = args.front();

	if (!who.empty()) {
		args.pop_front();
		user = nicks.find(who);
		if (user != nicks.end()) {
			// send info, maybe restirct info if user invisible
			who += " " + user->second.getUserName() + " " + user->second.getHostName() + " * :" + user->second.getRealName();
			ServerMessage(RPL_WHOISUSER, who + "\n");
			if (user->second.isOperator())
				ServerMessage(RPL_WHOISOPERATOR, user->second.getNick() + " :is a local operator\n");
			ServerMessage(RPL_WHOISSERVER, user->second.getNick() + " " + user->second.getServerName() + " :ft_ircserv\n");
			ServerMessage(RPL_WHOISACTUALLY, user->second.getNick() + " " + user->second.getHostName() + " :actually using host\n");
			ServerMessage(RPL_ENDOFWHOIS, ":end of WHOIS\n");
		} else {
			ServerMessage(ERR_NOSUCHNICK, ":nobody here by that name\n");
		}
	} else {
		ServerMessage(ERR_NONICKNAMEGIVEN, ":need nick name\n");
	}
}
