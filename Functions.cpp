#include "Functions.hpp"


Functions::Functions( ) : operPass("DamnSon"){

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
		if (nick.length() > 16)
			nick = nick.substr(0, 16);
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
	std::string mes = "CAP * LS :multi-prefix userhost-in-names\n";
	try {
		if (args.at(0) == "LS")
			send(fd, &mes[0], mes.length(), 0);
		if (args.at(0) == "REQ") {
			mes = "CAP * ACK " + args.at(1) + "\n";
			send(fd, &mes[0], mes.length(), 0);
		}
	} catch (std::exception &e) {
		ServerMessage(ERR_NEEDMOREPARAMS, " :Need more params\n");
	}
}

void Functions::ConnectionMessage( void ) {
	ServerMessage(RPL_WELCOME, " :Welcome You are now known as " + USER_FN(current_client->getNick(), current_client->getUserName(), current_client->getHostName()) + "\n" );
	ServerMessage(RPL_YOURHOST, " :Your host is " + current_client->getHostName() + "\n");
	ServerMessage(RPL_CREATED, " :This server was created some time recently\n");
	ServerMessage(RPL_MYINFO, current_client->getHostName() + "\n");
	ServerMessage(RPL_ISUPPORT, "MODES=2 MAXNICKLEN=16 NICKLEN=16 CHANNELLEN=50 CHANTYPES=# :are supported by this server\n");
}

void Functions::JOIN( void ) {
	if (current_client->isRegistered()) {
		ServerMessage(ERR_NOSUCHCHANNEL, args[0] + " :No such channel\n");
	} else
		ServerMessage(ERR_NOTREGISTERED, ":you must register first\n");
}

void Functions::RegisterUser( void ) {
	try {
		if (current_client->getUserName().empty())
			current_client->setUserName("~" + args.at(0));
		if (current_client->getHostName().empty())
			current_client->setHostName(args.at(1));
		current_client->setServerName(args.at(2));
		current_client->registration();
		current_client->setRealName(args.at(3));
		if (current_client->getNick().empty())
			ServerMessage(ERR_NONICKNAMEGIVEN, ":no nick name given\n");
		else {
			this->ConnectionMessage();
			this->MOTD();
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
	std::string modes = "+";
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
				if (target->second.isInvisibile())
					modes += "i";
				if (target->second.isOperator())
					modes += "o";
				ServerMessage(RPL_UMODEIS, modes + "\n");
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
	message += ":";
	message += args.front() + "\n";
	std::cout << message << std::endl;
	send(dest.getFD(), &message[0], message.length(), 0);
}

void Functions::PRIVMSG( void ) {
	try {
		args.at(0);
		args.at(1);
		try {
			//need to send to channels
			UsertoUser(*current_client, nicks.at(args[0]));
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
			UsertoUser(*current_client, nicks.at(args[0]));
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
	std::map<std::string, Client>::iterator cli_nick;
	std::string nick = current_client->getNick();
	cli_nick = nicks.find(nick);

	if (cli_nick != nicks.end())
		nicks.erase(cli_nick);
	close(fd);
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

void Functions::OPER(void)
{
	try
	{
		args.at(0);
		args.at(1);
		if (args[1] == operPass)
		{
			current_client->setOperator(true);
			ServerMessage(RPL_YOUREOPER, " : you did it\n");
		}
		else
			ServerMessage(ERR_PASSWDMISMATCH, " :das no good b\n");
	}
	catch (std::exception &e)
	{
		ServerMessage(ERR_NEEDMOREPARAMS, " :need more params\n");
	}
	return ;
}

void Functions::KILL(void)
{
	std::map<std::string, Client>::iterator user;

	try
	{
		args.at(0);
		args.at(1);
		user = nicks.find(args[0]);
		if (!current_client->isOperator())
			ServerMessage(ERR_NOPRIVILEGES, " :Permission Denied- You're not an IRC operator\n");
		else if (user == nicks.end())
			ServerMessage(ERR_NOSUCHNICK, " :Who dat?\n");
		else
		{
			killMsg(*current_client, user->second);
			quitMsg(user->second, "Killed (" + current_client->getNick() + "(" + args[1] + ")" + ")" + "\n" );
			errMsg(user, args[1]);
		}
	}
	catch (std::exception &e)
	{
		ServerMessage(ERR_NEEDMOREPARAMS, " :need more params\n");
	}
}

void Functions::killMsg(Client source, Client dest) {
	std::string message = ":" + USER_FN(source.getNick(), source.getUserName(), source.getHostName());
	message += " " + cmd + " " + dest.getNick() + " ";
	args.pop_front();
	message += args.front() + "\n";
	std::cout << message << std::endl;
	send(dest.getFD(), &message[0], message.length(), 0);
}

void Functions::quitMsg(Client source, std::string msg)
{
	std::string mes = ":" + USER_FN(source.getNick(), source.getUserName(), source.getHostName())
		+ "Quit :Quit: " + msg;
	send(source.getFD(), &mes[0], mes.length(), 0);
	//should be also sent to every user sharing a channel with source
}

void Functions::errMsg(std::map<std::string, Client>::iterator dest, std::string msg)
{
	std::string mes = ":" + dest->second.getServerName() + "Error: " + msg + "\n";
	send(dest->second.getFD(), &mes[0], mes.length(), 0);
	close(dest->second.getFD());
	nicks.erase(dest);
	// throw IrcErrorException(NULL);
}
