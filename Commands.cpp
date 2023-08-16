#include "Commands.hpp"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CONNECTION MESSAGES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void Commands::CAP( void ) {
	std::string mes = "CAP * LS :multi-prefix userhost-in-names\n";
	if (isEnoughParams(1)) {
		if (args[0] == "LS")
			current_client->pushSendBuff(mes);
		if (args[0] == "REQ") {
			mes = "CAP * ACK " + args[1] + "\n";
			current_client->pushSendBuff(mes);
		}
	}
}

void Commands::PASS( void ) {
	if (!current_client->isPassGood()) {
		try {
			if (args.at(0) == pass)
				current_client->passGood();
			else {
				ServerMessage(ERR_PASSWDMISMATCH, "04 :password doesn't match\n", *current_client);
				multi_cmd.clear();
			}
		} catch (std::exception &e) {
			ServerMessage(ERR_NEEDMOREPARAMS, "PASS :need more params\n", *current_client);
		}
	} else
		ServerMessage(ERR_ALREADYREGISTERED, ":you're already registered\n", *current_client);
}

void Commands::NICK( void ) {
	std:: string nick;
	if (args.size() < 1)
		ServerMessage(ERR_NONICKNAMEGIVEN, ":need to give a nick name\n", *current_client);
	else if (current_client->isPassGood()) {
		nick = args[0];
		if (checkNick(nick)) {
			if (nick.length() > 16)
				nick = nick.substr(0, 16);
			updateChannel(*current_client, current_client->getNick(), nick);
			UserMessage(cmd, " " + nick + " :" + nick + "\n", *current_client);
			nicks.erase(current_client->getNick());
			current_client->setNick(nick);
		}
	}
	else
		ServerMessage(ERR_PASSWDMISMATCH, " :need to give a password\n", *current_client);
}

void Commands::USER( void ) {
	if (!current_client->isRegistered()) {
		if (current_client->isPassGood()) {
			if (RegisterUser()) {
				MOTD();
				UserMessage("MODE", current_client->getNick() + " :+i\n", *current_client);
				current_client->setInvisibility(true);
			}
		}
		else
			ServerMessage(ERR_PASSWDMISMATCH, " :need to give password\n", *current_client);
	} else
		ServerMessage(ERR_ALREADYREGISTERED, " :you're already reistered\n", *current_client);
}

void Commands::PING( void ) {
	std::string pong = ":" + current_client->getServerName() + " PONG " + current_client->getServerName() + " :" + current_client->getServerName() + "\n";
	current_client->pushSendBuff(pong);
}

void Commands::OPER(void)
{
	if (isEnoughParams(2))
	{
		if (args[1] == serverOpPass)
		{
			current_client->setServerOp(true);
			ServerMessage(RPL_YOUREOPER, " : you did it\n", *current_client);
		}
		else
			ServerMessage(ERR_PASSWDMISMATCH, " :das no good b\n", *current_client);
	}
	return ;
}

void Commands::QUIT( void ) {
	client_it cli_nick;
	std::string nick = current_client->getNick();
	std::string reason;

	cli_nick = nicks.find(nick);
	if (args.size() > 0)
		reason = args[0];
	else
		reason = "";
	for (chan_it it = channels.begin(); it != channels.end(); it++) {
		if (it->second.isInChan(nick)) {
			it->second.echoToAll(*current_client, cmd, reason, true, sent);
			it->second.removeMember(*current_client);
		}
	}
	sent.clear();
	if (cli_nick != nicks.end())
		nicks.erase(cli_nick);
	close(fd);
	throw IrcErrorException("Client has quit\n");
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CHANNEL OPERATIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Commands::JOIN( void ) {
	std::vector<std::string> multi_Chan;
	std::string chanName;
	std::string pass;
	chan_it chan;

	if (isEnoughParams(1)) {
		if (args.size() > 1)
			pass = args[1];
		multi_Chan = split(args[0], ",");
		while (!multi_Chan.empty()) {
			chanName = multi_Chan.back();
			multi_Chan.pop_back();
			chan = channels.find(chanName);
			if (isChanName(chanName)) {
				if (chan == channels.end())
					channels[chanName] = Channel(chanName, *current_client);
				else
					chan->second.addMember(*current_client, pass);
			}
			else
				ServerMessage(ERR_BADCHANMASK, chanName + " :Bad Channel name\n", *current_client);
		}
	}
}

void Commands::PART(void)
{
	std::string reason;
	std::vector<std::string> multi_Chan;
	std::string chanName;
	chan_it chan;

	if (isEnoughParams(1))
	{
		multi_Chan = split(args[0], ",");
		while (!multi_Chan.empty()) {
			chanName = multi_Chan.back();
			multi_Chan.pop_back();
			chan = channels.find(chanName);
			if (channelExist(chanName, chan) && userInChan(chanName, chan))
			{
				if (args.size() >= 2)
					reason = args[1];
				else
					reason = "";
				UserMessage("PART", chanName + "\n", *current_client);
				chan->second.removeMember(*current_client);
				chan->second.echoToAll(*current_client, cmd, reason, true, sent);
				sent.clear();
				if (!chan->second.getCurrentCount())
					channels.erase(chan);
			}
		}
	}
}


void Commands::TOPIC(void)
{	
	if (isEnoughParams(1))
	{
		std::string chanName = args[0];
		chan_it chan = channels.find(chanName);
		if (channelExist(chanName, chan) && userInChan(chanName, chan))
		{
			if (args.size() == 2 && chan->second.isUserOp(chanName, *current_client))
			{
				UserMessage(cmd, chanName + " :" + args[1] + "\n", *current_client);
				chan->second.echoToAll(*current_client, cmd, ":" + args[1], true, sent);
				chan->second.setTopic(args[1]);
				sent.clear();
			}
			else
			{
				if (chan->second.hasTopic())
					ServerMessage(RPL_TOPIC, chanName + " :" + chan->second.getTopic() + "\n", *current_client);
				else
					ServerMessage(RPL_NOTOPIC, chanName + " :No topic set\n", *current_client);
			}
		}
	}
}

void Commands::INVITE(void)
{
	std::string chanName;
	client_it dest;
	chan_it chan;
	if (isEnoughParams(2))
	{
		chanName = args[1];
		dest = nicks.find(args[0]);
		chan = channels.find(chanName);
		if (dest == nicks.end())
			ServerMessage(ERR_NOSUCHNICK, args[0] + " :no such nick\n", *current_client);
		else if (channelExist(chanName, chan) && userInChan(chanName, chan))
		{
			//reject when channel is invite only and current user is not op
			if (chan->second.isInChan(dest->second.getNick()))
				ServerMessage(ERR_USERONCHANNEL, args[0] + " " + chanName + " :User already on channel\n", *current_client);
			else
			{
				ServerMessage(RPL_INVITING, args[0] + " " + chanName + "\n", *current_client);
				killMsg(*current_client, dest->second);
			}
		}
	}
}

// can have user names separated by commas --> DONE
void Commands::KICK(void)
{
	std::vector<std::string> multi_nick;
	std::string chanName;
	client_it dest;
	chan_it chan;
	std::string reason;
	
	if (isEnoughParams(2))
	{
		chanName = args[0];
		multi_nick = split(args[1], ",");
		while (!multi_nick.empty()) {
			dest = nicks.find(multi_nick.back());
			multi_nick.pop_back();
			if (dest == nicks.end())
				return ;
			chan = channels.find(chanName);
			reason = chan->second.getDefKickMsg();
			if (args.size() == 3)
				reason = args[2];
			if (channelExist(chanName, chan) && userInChan(chanName, chan))
			{
				if (!chan->second.isInChan(dest->second.getNick()))
					ServerMessage(ERR_USERNOTINCHANNEL, dest->second.getNick() + " " + chanName + " :User is not on channel\n", *current_client);
				else if (chan->second.isUserOp(chanName, *current_client))
				{
					UserMessage(cmd, chanName + " " + dest->second.getNick() + " " + reason + "\n", *current_client);
					chan->second.echoToAll(*current_client, cmd, dest->second.getNick() + " " + reason, true, sent);
					chan->second.removeMember(dest->second);
					sent.clear();
					if (!chan->second.getCurrentCount())
						channels.erase(chan);
				}
			}
		}
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SERVER QUERIES / COMMANDS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Commands::MOTD( void ) {
	std::fstream file;
	std::string tmp;

	file.open("ft_irc.motd");
	if (!file.is_open()) {
		ServerMessage(ERR_NOMOTD, ":no MOTD\n", *current_client);
	} else {
		ServerMessage(RPL_MOTDSTART, ":Message of the Day\n", *current_client);
		while (std::getline(file, tmp))
			ServerMessage(RPL_MOTD, tmp + "\n", *current_client);
		ServerMessage(RPL_ENDOFMOTD, ":End of /MOTD command.\a\n", *current_client);
	}
}

void Commands::MODE( void ) {
	std::string name;
	std::string modes;
	chan_it chan;
	// check for user modes, have to check same for channels
	if (isEnoughParams(1))
	{
		name = args[0];
		if (args.size() > 1)
			modes = args[1];
		if (!isChanName(args[0])) {
			userMode(modes, name);
		} else {
			chan = channels.find(name);
			if (channelExist(name, chan) && userInChan(name, chan) && chan->second.isUserOp(name, *current_client))
			{
				if (args.size() == 1)
					ServerMessage(RPL_CHANNELMODEIS, name + " " + chan->second.getModes() + "\n", *current_client);
			}
		}
	}

}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SENDING MESSAGES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// maybe check for comma list for nc -->DONE
void Commands::PRIVMSG( void ) {
	std::vector<std::string> multi_dest;
	std::string dest;
	chan_it chan;
	client_it client;
	
	if (isEnoughParams(2)) {
		multi_dest = split(args[0], ",");
		while (!multi_dest.empty()) {
			dest = multi_dest.back();
			multi_dest.pop_back();
			if (isChanName(dest)) {
				chan = channels.find(dest);
				if (chan != channels.end())
					chan->second.echoToAll(*current_client, cmd, args[1], true, sent);
				else
					ServerMessage(ERR_NOSUCHNICK, args[0] + " :no such nick/channel\n", *current_client);
				sent.clear();
			} else {
				client = nicks.find(dest);
				if (client != nicks.end())
					UsertoUser(*current_client, client->second, cmd, args[1] + "\n");
				else
					ServerMessage(ERR_NOSUCHNICK, ":" + dest + "\n", *current_client);
			}
		}
	} 
}

// can have them in comma list -->DONE
void Commands::NOTICE( void ) {
	std::vector<std::string> multi_dest;
	std::string dest;
	chan_it chan;
	client_it client;
	
	if (isEnoughParams(2)) {
		multi_dest = split(args[0], ",");
		while (!multi_dest.empty()) {
			dest = multi_dest.back();
			multi_dest.pop_back();
			if (isChanName(dest)) {
				chan = channels.find(dest);
				if (chan != channels.end())
					chan->second.echoToAll(*current_client, cmd, args[1], true, sent);
				sent.clear();
			} else {
				client = nicks.find(dest);
				if (client != nicks.end())
					UsertoUser(*current_client, client->second, cmd, args[1] + "\n");
			}
		}
	} 
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ USER-BASED QUERIES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Commands::WHOIS( void ) {
	client_it user;
	std::string who;

	if (args.size() >= 1)
		who = args.front();
	if (!who.empty()) {
		args.pop_front();
		user = nicks.find(who);
		if (user != nicks.end()) {
			// send info, maybe restirct info if user invisible
			who += " " + user->second.getUserName() + " " + user->second.getHostName() + " * :" + user->second.getRealName();
			ServerMessage(RPL_WHOISUSER, who + "\n", *current_client);
			if (user->second.isServerOp())
				ServerMessage(RPL_WHOISOPERATOR, user->second.getNick() + " :is a local operator\n", *current_client);
			ServerMessage(RPL_WHOISSERVER, user->second.getNick() + " " + user->second.getServerName() + " :ft_ircserv\n", *current_client);
			ServerMessage(RPL_WHOISACTUALLY, user->second.getNick() + " " + user->second.getHostName() + " :actually using host\n", *current_client);
			ServerMessage(RPL_ENDOFWHOIS, " :end of WHOIS\n",*current_client);
		} else
			ServerMessage(ERR_NOSUCHNICK, " :nobody here by that name\n", *current_client);
	} else
		ServerMessage(ERR_NONICKNAMEGIVEN, " :need nick name\n",  *current_client);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ OPERATOR MESSAGES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Commands::KILL(void)
{
	client_it user;

	if (isEnoughParams(2)) {
		user = nicks.find(args[0]);
		if (!current_client->isServerOp())
			ServerMessage(ERR_NOPRIVILEGES, " :Permission Denied- You're not an IRC operator\n", *current_client);
		else if (user == nicks.end())
			ServerMessage(ERR_NOSUCHNICK, " :Who dat?\n", *current_client);
		else
		{
			killMsg(*current_client, user->second);
			quitMsg(user->second, "Killed (" + current_client->getNick() + "(" + args[1] + ")" + ")" + "\n" );
			errMsg(user, args[1]);
		}
	} 
}
