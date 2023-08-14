#include "Commands.hpp"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CONNECTION MESSAGES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void Commands::CAP( void ) {
	std::string mes = "CAP * LS :multi-prefix userhost-in-names\n";
	try {
		if (args.at(0) == "LS")
			send(fd, mes.data(), mes.length(), 0);
		if (args.at(0) == "REQ") {
			mes = "CAP * ACK " + args.at(1) + "\n";
			send(fd, mes.data(), mes.length(), 0);
		}
	} catch (std::exception &e) {
		ServerMessage(ERR_NEEDMOREPARAMS, " :Need more params\n", *current_client);
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
	if (args.size() < 1)
		ServerMessage(ERR_NONICKNAMEGIVEN, ":need to give a nick name\n", *current_client);
	else if (current_client->isPassGood())
		this->addNick(args[0]);
	else
		ServerMessage(ERR_PASSWDMISMATCH, " :need to give a password\n", *current_client);
}

void Commands::USER( void ) {
	if (!current_client->isRegistered()) {
		if (current_client->isPassGood()) {
			this->RegisterUser();
		}
		else
			ServerMessage(ERR_PASSWDMISMATCH, " :need to give password\n", *current_client);
	} else
		ServerMessage(ERR_ALREADYREGISTERED, ":you're already reistered\n", *current_client);
}

void Commands::PING( void ) {
	std::string pong = ":" + current_client->getServerName() + " PONG " + current_client->getServerName() + " :" + current_client->getServerName() + "\n";
	send(fd, pong.data(), pong.length(), 0);
}

void Commands::OPER(void)
{
	if (isEnoughParams(2))
	{
		if (args[1] == operPass)
		{
			current_client->setOperator(true);
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
	size_t hash_pos;
	std::string chanName;
	chan_it chan;

	if (args.size() >= 1) {
		chanName = args[0];
		hash_pos = chanName.find('#');
		chan = channels.find(chanName);
		if (hash_pos == 0) {
			if (chan == channels.end())
				channels[chanName] = Channel(chanName, *current_client);
			else
				chan->second.addMember(*current_client);
		}
		else
			ServerMessage(ERR_BADCHANMASK, chanName + " :Bad Channel name\n", *current_client);
	} else {
		ServerMessage(ERR_NEEDMOREPARAMS, ":need more params\n", *current_client);
	}
}

void Commands::PART(void)
{
	std::string reason;
	
	//have to handle multiple channel parts
	/*this msg may be sent from a server to a client to notify the client that someone
	has been removed, in this case, source should be client being removed, and channel
	will be the channel they left/removed from. Server should distribute these multi
	channel part msg as a series of msgs with a single channel name with the reason*/
	if (isEnoughParams(1))
	{
		std::string chanName = args[0];
		chan_it chan = channels.find(chanName);
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
			if (args.size() == 2 && isUserOp(chanName))
			{
				//have to send to all in channel
				UserMessage(cmd, chanName + " :" + args[1] + "\n", *current_client);	
				chan->second.setTopic(args[1]);
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
		if (dest == nicks.end())
			return ;
		chan = channels.find(chanName);
		if (channelExist(chanName, chan) && userInChan(chanName, chan))
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

// void Commands::KICK(void)
// {
// 	
// }

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

// void Commands::MODE( void ) {

// }
void Commands::MODE( void ) {
	// check for user modes, have to check same for channels
	std::string modes = "+";
	try {
		client_it target = nicks.find(args.at(0));
		if (target == nicks.end())
			ServerMessage(ERR_NOSUCHNICK, ":" + args[0] + "\n", *current_client);
		else if (target->second.getFD() != fd)
			ServerMessage(ERR_USERSDONTMATCH, " :Can't touch this\n", *current_client);
		else {
			try {
				args.at(1);
				ServerMessage(ERR_UMODEUNKOWNFLAG, ":Unknown MODE flag " + args[1] + "\n", *current_client);
			} catch (std::exception &e) {
				if (target->second.isInvisibile())
					modes += "i";
				if (target->second.isOperator())
					modes += "o";
				ServerMessage(RPL_UMODEIS, modes + "\n", *current_client);
			}
		}
	} catch (std::exception &e) {
		ServerMessage(ERR_NEEDMOREPARAMS, ":" + cmd + " need more params\n", *current_client);
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SENDING MESSAGES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Commands::PRIVMSG( void ) {
	if (args.size() >= 2) {
		size_t hash_pos = args[0].find('#');
		if (hash_pos == 0) {
			chan_it chan = channels.find(args[0]);
			if (chan != channels.end())
				chan->second.echoToAll(*current_client, cmd, args[1], true, sent);
			else
				ServerMessage(ERR_NOSUCHCHANNEL, " :no such channel\n", *current_client);
			sent.clear();
		} else {
			try {
				UsertoUser(*current_client, nicks.at(args[0]), cmd, args[1] + "\n");
			} catch (std::exception &e) {
				ServerMessage(ERR_NOSUCHNICK, ":" + args[0] + "\n", *current_client);
			}
		}
	} else
		ServerMessage(ERR_NEEDMOREPARAMS, " :need more params\n", *current_client);
}

void Commands::NOTICE( void ) {
	if (args.size() >= 2) {
		size_t hash_pos = args[0].find('#');
		if (hash_pos == 0) {
			chan_it chan = channels.find(args[0]);
			if (chan != channels.end())
				chan->second.echoToAll(*current_client, cmd, args[1], true, sent);
			sent.clear();
		} else {
			try {
				UsertoUser(*current_client, nicks.at(args[0]), cmd, args[1] + "\n");
			} catch (std::exception &e) {
				std::cout << "Error: notice not sent\n";
			}
		}
	} else
		ServerMessage(ERR_NEEDMOREPARAMS, " :need more params\n", *current_client);
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
			if (user->second.isOperator())
				ServerMessage(RPL_WHOISOPERATOR, user->second.getNick() + " :is a local operator\n", *current_client);
			ServerMessage(RPL_WHOISSERVER, user->second.getNick() + " " + user->second.getServerName() + " :ft_ircserv\n", *current_client);
			ServerMessage(RPL_WHOISACTUALLY, user->second.getNick() + " " + user->second.getHostName() + " :actually using host\n", *current_client);
			ServerMessage(RPL_ENDOFWHOIS, ":end of WHOIS\n",*current_client);
		} else {
			ServerMessage(ERR_NOSUCHNICK, ":nobody here by that name\n", *current_client);
		}
	} else {
		ServerMessage(ERR_NONICKNAMEGIVEN, ":need nick name\n",  *current_client);
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ OPERATOR MESSAGES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Commands::KILL(void)
{
	client_it user;

	if (args.size() >= 2) {
		user = nicks.find(args[0]);
		if (!current_client->isOperator())
			ServerMessage(ERR_NOPRIVILEGES, " :Permission Denied- You're not an IRC operator\n", *current_client);
		else if (user == nicks.end())
			ServerMessage(ERR_NOSUCHNICK, " :Who dat?\n", *current_client);
		else
		{
			killMsg(*current_client, user->second);
			quitMsg(user->second, "Killed (" + current_client->getNick() + "(" + args[1] + ")" + ")" + "\n" );
			errMsg(user, args[1]);
		}
	} else {
		ServerMessage(ERR_NEEDMOREPARAMS, " :need more params\n", *current_client);
	}
}
