#include "Functions.hpp"
#include "Commands.hpp"


Functions::Functions( ) : serverOpPass("DamnSon"){

}

Functions::~Functions( ) {

}

void Functions::setPass( std::string pass ) {
	this->pass = pass;
}

std::string Functions::getPass( void ) const {
	return this->pass;
}

bool Functions::checkNick( std::string nick ) {
	client_it it;

	it = nicks.find(nick);
	if (nick.find_first_of("#&\x03") != nick.npos)
		ServerMessage(ERR_ERRONEUSNICKNAME, ":" + nick + " erroneus nick name\n", *current_client);
	else if (it != nicks.end()) {
		if (current_client->isRegistered())
			ServerMessage(ERR_NICKNAMEINUSE, nick + " :Nickname is already in use\n", *current_client);
		else {
			ServerMessage("ERROR ", "13 your nick is unavailable, get a better name and try connecting again\n", *current_client);
			multi_cmd.clear();
			// close(fd);
			throw IrcErrorException("user tried to register with nick already in use\n");
		}
		return false;
	}
	return true;
}

void Functions::updateChannel( Client &client, std::string old_nick, std::string new_nick ) {
	for(chan_it it = channels.begin(); it != channels.end(); it++) {
		if (it->second.isInChan(old_nick)) {
			it->second.updateMemberNick(client, old_nick, new_nick);
			it->second.echoToAll(*current_client, cmd, ":" + new_nick, false, sent);
		}
	}
	sent.clear();
}

bool Functions::RegisterUser( void ) {
	try {
		if (current_client->getUserName().empty())
			current_client->setUserName("~" + args.at(0));
		if (current_client->getHostName().empty())
			current_client->setHostName(args.at(1));
		current_client->setServerName(args.at(2));
		current_client->setRealName(args.at(3));
		if (current_client->getNick().empty())
			ServerMessage(ERR_NONICKNAMEGIVEN, ":no nick name given\n", *current_client);
		else {
			current_client->registration();
			ConnectionMessage(*current_client);
			return true;
		}
	} catch (std::exception &e) {
		ServerMessage(ERR_NEEDMOREPARAMS, ":need more params\n", *current_client);
		nicks.erase(current_client->getNick());
		// close(fd);
		current_client->set_removal(true);
		throw IrcErrorException("user not registered\n");
	}
	return false;
}

void Functions::killMsg(Client &source, Client &dest) {
	std::string message = ":" + USER_FN(source.getNick(), source.getUserName(), source.getHostName());
	message += " " + cmd + " " + dest.getNick() + " " + args[1] + "\n";
	dest.pushSendBuff(message);
}

void Functions::quitMsg(Client source, std::string msg)
{
	std::string nick = source.getNick();
	std::string user_info = USER_FN(source.getNick(), source.getUserName(), source.getHostName());
	std::string mes = ":" + user_info + " QUIT :Quit: " + msg;
	source.pushSendBuff(mes);
	for (chan_it it = channels.begin(); it != channels.end(); it++) {
		if (it->second.isInChan(nick)) {
			it->second.echoToAll(source, "", mes, false, sent);
			it->second.removeMember(source);
		}
	}
	sent.clear();
}

void Functions::errMsg(client_it dest, std::string msg)
{
	std::string mes = ":" + dest->second->getServerName() + "Error: " + msg + "\n";
	dest->second->pushSendBuff(mes);
	// close(dest->second.getFD());
	dest->second->set_removal(true);
	nicks.erase(dest);
}

std::vector<std::string> Functions::split(std::string str, std::string delim) {
	std::vector<std::string> ret;
	size_t pos;
	while (!str.empty()) {
		pos = str.find_first_of(delim);
		ret.push_back(str.substr(0, pos));
		pos = str.find_first_not_of(delim, pos);
		str.erase(0, pos);
	}
	return ret;
}

bool Functions::isEnoughParams(unsigned int paramNum)
{
	if (args.size() < paramNum)
	{
		ServerMessage(ERR_NEEDMOREPARAMS, " :need more params\n", *current_client);
		return false;
	}
	return true;
}

bool Functions::channelExist(std::string chanName, chan_it channel)
{
	if (channel == channels.end())
	{
		ServerMessage(ERR_NOSUCHCHANNEL, chanName + " :No such channel\n", *current_client);
		return false;
	}
	return true;
}

bool Functions::userInChan(std::string chanName, chan_it channel)
{
	if (!channel->second.isInChan(current_client->getNick()))
	{
		ServerMessage(ERR_NOTONCHANNEL, chanName + " :You're not on that channel\n", *current_client);
		return false;
	}
	return true;
}

bool Functions::isChanName( std::string name )
{
	if (name.find('#') == 0)
		return true;
	return false;
}

void Functions::userMode(std::string modes, std::string name)
{
	try {
		std::cout << name << std::endl;
		client_it target = nicks.find(args.at(0));
		if (target == nicks.end())
			ServerMessage(ERR_NOSUCHNICK, ":" + args[0] + "\n", *current_client);
		else if (target->second->getFD() != fd)
			ServerMessage(ERR_USERSDONTMATCH, " :Can't touch this\n", *current_client);
		else {
			try {
				if (args.at(1).find("i") != std::string::npos) {
					if (modes.find("-") != modes.npos && current_client->isInvisible()) {
						current_client->setInvisibility(false);
						UserMessage(cmd, args[0] + " :" + args[1] + "\n", *current_client);
					}
					else if (modes.find("+") != modes.npos && !current_client->isInvisible()) {
						current_client->setInvisibility(true);
						UserMessage(cmd, args[0] + " :" + args[1] + "\n", *current_client);
					}
				} else if (args.at(1).find("i") == std::string::npos)
					ServerMessage(ERR_UMODEUNKOWNFLAG, ":Unknown MODE flag " + args.at(1) + "\n", *current_client);
			} catch (std::exception &e) {
				if (target->second->isInvisible())
					modes += "i";
				if (target->second->isServerOp())
					modes += "o";
				ServerMessage(RPL_UMODEIS, modes + "\n", *current_client);
			}
		}
	} catch (std::exception &e) {
		ServerMessage(ERR_NEEDMOREPARAMS, ":" + cmd + " need more params\n", *current_client);
	}
}

void Functions::channelMode(std::string modes, chan_it chan) {
	devector<std::string> arguments;
	std::string mode;
	std::string trailing;
	size_t		mode_pos;
	size_t		mode_end_pos;
	size_t		sign_pos;
	size_t		err;

	err = modes.find_first_not_of("+-oilkt");
	mode = modes[err];
	if (err == modes.npos) {
		for (size_t i = 2; i < args.size(); i++)
			arguments.push_back(args[i]);
		mode_pos = modes.find_first_not_of("+-");
		mode_end_pos = modes.find_first_of("+-", mode_pos);
		sign_pos = mode_pos - 1;
		mode = modes[sign_pos];
		while (mode_pos < modes.length()) {
			chan->second.chanModes(modes[mode_pos], modes[sign_pos], arguments, *current_client, mode, trailing);
			// mode += modes[mode_pos];
			mode_pos++;
			if (mode_pos == mode_end_pos) {
				mode_pos = modes.find_first_not_of("+-", mode_end_pos);
				mode_end_pos = modes.find_first_of("+-", mode_pos);
				sign_pos = mode_pos - 1;
				mode += modes[sign_pos];
			}
		}
		mode += trailing;
		current_client->pushSendBuff(chan->second.echoToAll(*current_client, cmd, mode, true, sent));
		sent.clear();
	} else
		ServerMessage(ERR_UNKNOWNMODE, mode + " :is an unknown mode to me\n", *current_client);
}
