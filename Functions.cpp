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
			// this->QUIT();
			close(fd);
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

Client* Functions::findClient(const std::string& nick) {
    client_iterator it = nicks.find(nick);
    if (it != nicks.end()) {
        return it->second;
    }
    return NULL; // Client with the given nick not found
}

// Channels

const std::map<std::string, Channel*>&	Functions::getAllChannels() { return channels; }

void Functions::addChannel(const std::string& channelName, Channel* channel) {
    channels[channelName] = channel;
}

Channel* Functions::findChannel(const std::string& channelName) {
    channel_iterator it = channels.find(channelName);
    if (it != channels.end()) {
        return it->second;
    }
    return NULL;
}

void Functions::removeChannel(const std::string& channelName) {
    Channel* channel = findChannel(channelName);
    if (channel) {
        delete channel;
        channels.erase(channelName);
    }
}

void Functions::JOIN(void)
{
	//current_client = person sending message
	// cmd = command like NICK JOIN
	// args = arguments
	// args[0] = channel name
	// args[1] = pass 
    Channel* channel = findChannel(args[0]); 

    if (!channel) {
        channel = new Channel(args[0], current_client, args[1]);
        addChannel(args[0], channel);
        current_client->joinChannel(args[0], channel);
        std::string joinMessage = "JOIN " + args[0];
        current_client->write(joinMessage);

        std::string topicMessage = "332 " + current_client->getNick() + " " + args[0] + " :" + channel->getTopic();
        current_client->write(topicMessage);
    } else {
        if (!channel->isInviteOnly() || channel->isInvited(current_client)) {
            if (channel->checkPassword(args[1])) {
                channel->addMember(current_client);
                current_client->joinChannel(args[0], channel);
                std::string joinMessage = "JOIN " + args[0];
                current_client->write(joinMessage);

                std::string topicMessage = "332 " + current_client->getNick() + " " + args[0] + " :" + channel->getTopic();
                current_client->write(topicMessage);
            } else {
                std::string errorMessage = "475 " + current_client->getNick() + " " + args[0] + " :Cannot join channel (bad key)";
                current_client->write(errorMessage);
            }
        } else {
            std::string errorMessage = "473 " + current_client->getNick() + " " + args[0] + " :Cannot join channel (+i)";
            current_client->write(errorMessage);
        }
    }
}

void Functions::PART(Client* client, const std::string& channelName) {
	//current_client = person sending message
	// cmd = command like NICK JOIN
	// args = arguments
	// args[0] = channel name
    Channel* channel = findChannel(args[0]); 

    if (channel) {
        // Check if the client is the channel owner
        if (channel->getOwner() == current_client) {
            // The channel owner cannot PART the channel, need to use the KICK command
            current_client->write("482 " + current_client->getNick() + " " + args[0] + 
						" :You're the channel owner, use KICK to leave the channel.");
            return;
        }

        // Check if the client is an operator or a member of the channel
        if (channel->isOperator(current_client) || channel->isMember(current_client)) {
            // Remove the client from the channel
            channel->removeMember(current_client);
            channel->removeOperator(current_client); // Remove from operators if client is an operator

            // Send a PART message to the client
            std::string partMessage = "PART " + args[0];
            current_client->write(partMessage);

            // Send a PART message to other channel members
            std::string broadcastMessage = ":" + current_client->getNick() + "!" + current_client->getUsername() + 
						"@" + current_client->getHostname() + " PART " + args[0];
            channel->broadcast(broadcastMessage, current_client);

            // Remove the channel if it becomes empty
            if (channel->isEmpty()) {
                removeChannel(args[0]);
                delete channel;
            }
        } else {
            // Send an error message if the client is not a member or operator
            std::string errorMessage = "442 " + current_client->getNick() + " " + args[0] + " :You're not on that channel";
            current_client->write(errorMessage);
        }
    } else {
        // Send an error message if the channel doesn't exist
        std::string errorMessage = "403 " + current_client->getNick() + " " + args[0] + " :No such channel";
        current_client->write(errorMessage);
    }
}

void Functions::INVITE(Client* client, const std::string& nick, const std::string& channelName) {
	//current_client = person sending message
	// cmd = command like NICK JOIN
	// args = arguments
	// args[0] = nick name
	// args[1] = channel name 
    Channel* channel = findChannel(args[1]);

    if (channel) {
        // Check if the client is the channel owner or operator
        if (channel->isOperator(client) || channel->getOwner() == client) {
            // Find the target client by nickname
            Client* targetClient = findClient(nick);

            if (targetClient) {
                // Check if the target client is already a member or invited to the channel
                if (channel->isMember(targetClient) || channel->isInvited(targetClient)) {
                    client->write("443 " + client->getNick() + " " + nick + " " + args[1] + " :is already on channel");
                } else {
                    // Add the target client to the invited list of the channel
                    channel->addInvited(targetClient);
                    targetClient->write(":" + client->getNick() + " INVITE " + targetClient->getNick() + " :" + args[1]);
                }
            } else {
                client->write("401 " + client->getNick() + " " + nick + " :No such nick");
            }
        } else {
            client->write("482 " + client->getNick() + " " + args[1] + " :You're not channel operator");
        }
    } else {
        client->write("403 " + client->getNick() + " " + args[1] + " :No such channel");
    }
}

void Functions::KICK(Client* client, const std::string& channelName, const std::string& targetNick, const std::string& reason) {
    Channel* channel = findChannel(channelName);

    if (channel) {
        // Check if the client has kick privileges (owner or operator)
        if (channel->isOperator(client) || channel->getOwner() == client) {
            Client* target = findClient(targetNick);

            if (target) {
                // Check if the target is the channel owner
                if (target == channel->getOwner()) {
                    client->write("482 " + client->getNick() + " " + channelName + " :You cannot kick the channel owner.");
                    return;
                }

                // Perform the kick
                channel->kick(client, target, reason);
            } else {
                // Send an error message if the target client doesn't exist
                std::string errorMessage = "401 " + client->getNick() + " " + targetNick + " :No such nick";
                client->write(errorMessage);
            }
        } else {
            // Send an error message if the client doesn't have kick privileges
            std::string errorMessage = "482 " + client->getNick() + " " + channelName + " :You're not a channel operator";
            client->write(errorMessage);
        }
    } else {
        // Send an error message if the channel doesn't exist
        std::string errorMessage = "403 " + client->getNick() + " " + channelName + " :No such channel";
        client->write(errorMessage);
    }
}

void Functions::TOPIC(Client* client, const std::string& channelName, const std::string& newTopic) {
    Channel* channel = findChannel(channelName);

    if (channel) {
        // Check if the client is a member or operator of the channel
        if (channel->isOperator(client) || channel->isMember(client)) {
            // Set the new topic for the channel
            channel->setTopic(newTopic);

            // Send the new topic to the channel
            std::string topicMessage = "TOPIC " + channelName + " :" + newTopic;
            channel->broadcast(topicMessage);

            // Send a response to the client
            std::string responseMessage = "332 " + client->getNick() + " " + channelName + " :" + newTopic;
            client->write(responseMessage);
        } else {
            // Send an error message if the client is not a member or operator
            std::string errorMessage = "442 " + client->getNick() + " " + channelName + " :You're not on that channel";
            client->write(errorMessage);
        }
    } else {
        // Send an error message if the channel doesn't exist
        std::string errorMessage = "403 " + client->getNick() + " " + channelName + " :No such channel";
        client->write(errorMessage);
    }
}

void Functions::MODE(Client* client, const std::string& channelName, const std::string& modes, const std::string& parameters) {
    Channel* channel = findChannel(channelName);

    if (!channel) {
        // Send an error message if the channel doesn't exist
        std::string errorMessage = "403 " + client->getNick() + " " + channelName + " :No such channel";
        client->write(errorMessage);
        return;
    }

    // Check if the client has permission to change modes
    if (!channel->isOperator(client) && channel->getOwner() != client) {
        // Send an error message if the client is not an operator or the owner
        std::string errorMessage = "482 " + client->getNick() + " " + channelName + " :You're not a channel operator or owner";
        client->write(errorMessage);
        return;
    }

    std::string modeChanges;
    std::string parametersToSend;
    
    for (size_t i = 0; i < modes.length(); ++i) {
        char mode = modes[i];
        char sign = '+';

        if (mode == '+' || mode == '-') {
            sign = mode;
            continue;
        }

        // Check for supported channel modes
        if (mode == 'i' || mode == 't' || mode == 'k' || mode == 'o' || mode == 'l') {
            if (sign == '-') {
                // Handle removing modes here
                if (mode == 'o') {
                    // Handle removing operator mode
                    std::string targetNickname = getNextParameter(parameters);
					Client* targetClient = findClient(targetNickname);
					if (!targetClient) {
						// Send an error message if the target client doesn't exist
						std::string errorMessage = "401 " + client->getNick() + " " + targetNickname + " :No such nick/channel";
						client->write(errorMessage);
						return;
					}
					// Check if the target is an operator and remove the operator mode
					if (channel->isOperator(targetClient)) {
						channel->removeOperator(targetClient);
						std::string modeMessage = "MODE " + channelName + " -o " + targetNickname;
						channel->broadcast(modeMessage);
					} else {
						// Send an error message if the target is not an operator
						std::string errorMessage = "482 " + client->getNick() + " " + targetNickname + " :You're not a channel operator";
						client->write(errorMessage);
						return;
					}
				} else if (mode == 'i') {
					// Handle removing invite-only mode
					if (channel->isOwner(client)) {
						if (channel->isInviteOnly()) {
							channel->setInviteOnly(false);
							std::string modeMessage = "MODE " + channelName + " -i";
							channel->broadcast(modeMessage);
						}
					} else {
						// Send an error message if the client is not the channel owner
						std::string errorMessage = "482 " + client->getNick() + " " + channelName + " :You're not the channel owner";
						client->write(errorMessage);
						return;
					}
                } else if (mode == 'l') {
                    // Handle removing user limit mode
					if (channel->isOwner(client)) {
						if (channel->hasUserLimit()) {
							channel->removeUserLimit();
							std::string modeMessage = "MODE " + channelName + " -l";
							channel->broadcast(modeMessage);
						}
					} else {
						// Send an error message if the client is not the channel owner
						std::string errorMessage = "482 " + client->getNick() + " " + channelName + " :You're not the channel owner";
						client->write(errorMessage);
						return;
					}
				} else if (mode == 'k') {
					// Handle removing channel key mode
					if (channel->isOwner(client)) {
						if (channel->hasChannelKey()) {
							channel->removeChannelKey();
							std::string modeMessage = "MODE " + channelName + " -k";
							channel->broadcast(modeMessage);
						}
					} else {
						// Send an error message if the client is not the channel owner
						std::string errorMessage = "482 " + client->getNick() + " " + channelName + " :You're not the channel owner";
						client->write(errorMessage);
						return;
					}
				} else if (mode == 't') {
					// Handle removing topic restrictions mode
					if (channel->isOwner(client)) {
						if (channel->hasTopicRestrictions()) {
							channel->removeTopicRestrictions();
							std::string modeMessage = "MODE " + channelName + " -t";
							channel->broadcast(modeMessage);
						}
					} else {
						// Send an error message if the client is not the channel owner
						std::string errorMessage = "482 " + client->getNick() + " " + channelName + " :You're not the channel owner";
						client->write(errorMessage);
						return;
					}
                } else {
                    // Unsupported mode for removal, send an error message
                    std::string errorMessage = "472 " + client->getNick() + " " + channelName + " " + mode + " :Unknown mode";
                    client->write(errorMessage);
                    return;
                }
            } else {
                // Handle adding modes here
                if (mode == 'i') {
                    // Handle invite-only mode
                    if (channel->isOwner(client)) {
					if (!channel->isInviteOnly()) {
						channel->setInviteOnly(true);
						std::string modeMessage = "MODE " + channelName + " +i";
						channel->broadcast(modeMessage);
					}
				} else {
					// Send an error message if the client is not the channel owner
					std::string errorMessage = "482 " + client->getNick() + " " + channelName + " :You're not the channel owner";
					client->write(errorMessage);
					return;
				}
                } else if (mode == 't') {
					// Handle topic restrictions mode
					if (channel->isOwner(client)) {
						// Apply topic restrictions to the channel
						// ...
						std::string modeMessage = "MODE " + channelName + " +t";
						channel->broadcast(modeMessage);
					} else {
						// Send an error message if the client is not the channel owner
						std::string errorMessage = "482 " + client->getNick() + " " + channelName + " :You're not the channel owner";
						client->write(errorMessage);
						return;
					}
                } else if (mode == 'k') {
					// Handle channel key mode
					if (channel->isOwner(client)) {
						// Parse the channel key argument and set it for the channel
						// ...
						std::string modeMessage = "MODE " + channelName + " +k " + channelKeyArg;
						channel->broadcast(modeMessage);
					} else {
						// Send an error message if the client is not the channel owner
						std::string errorMessage = "482 " + client->getNick() + " " + channelName + " :You're not the channel owner";
						client->write(errorMessage);
						return;
					}
                } else if (mode == 'o') {
                    // Handle operator mode
                    std::string targetNickname = getNextParameter(parameters);
					Client* targetClient = findClient(targetNickname);
					if (!targetClient) {
						// Send an error message if the target client doesn't exist
						std::string errorMessage = "401 " + client->getNick() + " " + targetNickname + " :No such nick/channel";
						client->write(errorMessage);
						return;
					}
					// Check if the target is not already an operator and add the operator mode
					if (!channel->isOperator(targetClient)) {
						channel->addOperator(targetClient);
						std::string modeMessage = "MODE " + channelName + " +o " + targetNickname;
						channel->broadcast(modeMessage);
					}
                } else if (mode == 'l') {
					// Handle user limit mode
					if (channel->isOwner(client)) {
						// Parse the user limit argument and set it for the channel
						// ...
						std::string modeMessage = "MODE " + channelName + " +l " + userLimitArg;
						channel->broadcast(modeMessage);
					} else {
						// Send an error message if the client is not the channel owner
						std::string errorMessage = "482 " + client->getNick() + " " + channelName + " :You're not the channel owner";
						client->write(errorMessage);
						return;
					}
                } else {
                    // Unsupported mode for addition, send an error message
                    std::string errorMessage = "472 " + client->getNick() + " " + channelName + " " + mode + " :Unknown mode";
                    client->write(errorMessage);
                    return;
                }
            }
        } else {
            // Unsupported mode, send an error message
            std::string errorMessage = "472 " + client->getNick() + " " + channelName + " " + mode + " :Unknown mode";
            client->write(errorMessage);
            return;
        }
    }

    // Send mode changes to the channel
    std::string modeMessage = "MODE " + channelName + " " + modeChanges + " " + parametersToSend;
    channel->broadcast(modeMessage);

    // Optionally, send a reply to the client confirming the mode changes
    std::string replyMessage = "MODE " + channelName + " " + modeChanges + " " + parametersToSend;
    client->write(replyMessage);
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
	std::string who;

	if (args.size() >= 1)
		who = args.front();
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
	if (args.size() >= 2)
	{
		if (args[1] == operPass)
		{
			current_client->setOperator(true);
			ServerMessage(RPL_YOUREOPER, " : you did it\n");
		}
		else
			ServerMessage(ERR_PASSWDMISMATCH, " :das no good b\n");
	} else {
		ServerMessage(ERR_NEEDMOREPARAMS, " :need more params\n");
	}
	return ;
}

void Functions::KILL(void)
{
	std::map<std::string, Client>::iterator user;

	if (args.size() >= 2) {
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
	} else {
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
	throw IrcErrorException(NULL);
}
