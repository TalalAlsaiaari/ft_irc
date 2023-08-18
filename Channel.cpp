#include "Channel.hpp"

Channel::Channel() {
}

Channel::Channel( std::string name, Client &creator ) {
    this->name = name;
    if (name.length() > 50)
        this->name = name.substr(0, 50);
    this->topic = "";
    this->limit = 0;
    this->inviteOnly = false;
    this->hasLimit = false;
    this->hasKey = false;
    this->defKickMsg = "Bye, miss you";
    this->currentCount = 1;
    this->modes = "";
    operators[creator.getNick()] = &creator;
    UserMessage("JOIN", name + " * :welcome\n", creator);
    whoIsChan(creator);
}

Channel::~Channel() {

}

void Channel::addMember( Client &add, std::string key ) {
    std::map<std::string, Client *> sent;

    if (!isInChan(add.getNick()) && checkEntrance(add.getNick(), add, key)) {
        members[add.getNick()] = &add;
        UserMessage("JOIN", name + " * :welcome\n", add);
        if (hasTopic())
            ServerMessage(RPL_TOPIC, name + " :" + topic + "\n", add);
        whoIsChan(add);
        echoToAll(add, "JOIN", "", true, sent);
        removeInvite(add.getNick());
        this->currentCount++;
    }
}

std::string Channel::echoToAll(Client &client, std::string cmd, std::string trailing, bool chan, std::map<std::string, Client *> &sent) {
    std::string client_info = USER_FN(client.getNick(), client.getUserName(), client.getHostName());
    std::string message = ":" + client_info + " " + cmd;

    if (chan)
        message += " " + name;
    if (!trailing.empty())
        message += " " + trailing;
    message += "\n";
    for (iter it = members.begin(); it != members.end(); it++) {
        if (it->second->getNick() != client.getNick() && sent.find(it->second->getNick()) == sent.end()) {
            it->second->pushSendBuff(message);
            sent[it->second->getNick()] = it->second;
        }
    }
    for (iter it = operators.begin(); it != operators.end(); it++) {
        if (it->second->getNick() != client.getNick() && sent.find(it->second->getNick()) == sent.end()) {
            it->second->pushSendBuff(message);
            sent[it->second->getNick()] = it->second;
        }
    }
    return message;
}

// @time=2023-08-16T14:28:51.727Z :aballers!~user@5.195.225.158 MODE #newtest42 +o aballiscool
void Channel::makeChanOp( Client &src, Client &dst ) {
    iter member = members.find(dst.getNick());

    if (isInChan(dst.getNick())) {
        if (member != members.end())
            members.erase(member);
        operators[dst.getNick()] = &dst;
    } else
        ServerMessage(ERR_USERNOTINCHANNEL, dst.getNick() + " " + name + " :Not on channel\n", src);
}

void Channel::removeMember( Client & remove) {
    iter member = members.find(remove.getNick());
    iter oper = operators.find(remove.getNick());

    if (member != members.end())
    {
        members.erase(member);
        this->currentCount--;
    }
    if (oper != operators.end())
    {
        operators.erase(oper);
        this->currentCount--;
    }
}

void Channel::setTopic(std::string topic)
{
    this->topic = topic;
    return ;
}

bool Channel::isInChan( std::string Nick ) {
    iter member;
    iter oper;

    oper = operators.find(Nick);
    member = members.find(Nick);
    if (member == members.end() && oper == operators.end())
        return false;
    return true;
}

bool Channel::isInvited( std::string Nick ) {
    iter invite;

    invite = invited.find(Nick);
    if (invite == invited.end())
        return false;
    return true;
}

void Channel::addInvite( std::string nick, Client &client ) {
    invited[nick] = &client;
}

void Channel::removeInvite( std::string nick ) {
    iter invite;

    invite = invited.find(nick);
    if (invite != invited.end())
        invited.erase(invite);
}

bool Channel::isInviteOnly( void ) {
    return inviteOnly;
}

void Channel::whoIsChan( Client &client ) {
    std::string who = "@ " + name + " :";
    for (iter it = members.begin(); it != members.end(); it++) {
            who += it->second->getNick() + " ";
    }
    for (iter it = operators.begin(); it != operators.end(); it++) {
            who +=  "@" + it->second->getNick() + " ";
    }
    who += "\n";
    ServerMessage(RPL_NAMREPLY, who, client);
    ServerMessage(RPL_ENDOFNAMES, name + " :END of /NAMES list\n", client);
}

bool Channel::hasTopic(void)
{
    if (!this->topic.empty())
        return true;
    return false;
}

std::string const Channel::getTopic(void) const
{
    return this->topic;
}

void Channel::updateMemberNick( Client &client, std::string old_nick, std::string new_nick ) {
    iter mems = members.find(old_nick);
    iter oper = operators.find(old_nick);
    iter invite = invited.find(old_nick);
    
    if (mems != members.end()) {
        members.erase(old_nick);
        members[new_nick] = &client;
    }
    if (oper != operators.end()) {
        operators.erase(old_nick);
        operators[new_nick] = &client;
    }
    if (invite != invited.end()) {
        invited.erase(old_nick);
        invited[new_nick] = &client;
    }
}

void Channel::modeI(char mode, char sign) {
    if (mode == 'i') {
        if (sign == '+') {
            inviteOnly = true;
            setModes(mode);
        }
        if (sign == '-') {
            inviteOnly = false;
            removeModes(mode);
        }
    }
}

// void Channel::modeO(std::string mode, std::string sign, devector<std::string> &args) {

// }

// void Channel::modeK(std::string mode, std::string sign, devector<std::string> &args) {

// }

// void Channel::modeL(std::string mode, std::string sign, devector<std::string> &args) {

// }

// void Channel::modeT(std::string mode, std::string sign) {

// }

unsigned int Channel::getCurrentCount(void) const
{
    return this->currentCount;
}

std::string const Channel::getDefKickMsg(void) const
{
    return this->defKickMsg;
}

std::string const Channel::getModes(void) const
{
    return this->modes;
}

void Channel::setInviteOnly( bool invite ) {
    this->inviteOnly = invite;
}

void Channel::setModes(char mode) {
    size_t pos = modes.find(mode);
    if (pos == modes.npos) 
        modes += mode;
}

void Channel::removeModes(char mode) {
    size_t pos = modes.find(mode);
    if (pos != modes.npos)
        modes.erase(pos);
}

bool Channel::checkEntrance( std::string nick, Client &client, std::string key ) {
    if (hasLimit) {
        if (currentCount >= limit) {
            ServerMessage(ERR_CHANNELISFULL, name + " :Cannot join channel (+l) - channel is full, try again later", client);
            return false;
        }
    }
    if (inviteOnly) {
        if (!isInvited(nick)) {
            ServerMessage(ERR_INVITEONLYCHAN, name + " :Cannot join channel (+i) - you must be invited\n", client);
            return false;
        }
    }
    if (hasKey) {
        if (key != pass) {
            ServerMessage(ERR_BADCHANNELKEY, name + " :Cannot join channel (+k) - bad key", client);
            return false;
        }
    }
    return true;
}

void Channel::setChannelOp(Client &target)
{
    operators[target.getNick()] = &target;
}

bool Channel::isUserOp(std::string chanName, Client& user)
{
	if (operators.find(user.getNick()) == operators.end()) //should check for channel op
	{
		ServerMessage(ERR_CHANOPRIVSNEEDED, chanName + " :You're not a channel operator\n", user);
		return false;
	}
	return true;
}