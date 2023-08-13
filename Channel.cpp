#include "Channel.hpp"

Channel::Channel() {
}

Channel::Channel( std::string name, Client &creator ) {
    this->name = name;
    // this->pass = pass;
    this->topic = "";
    this->limit = 0;
    this->inviteOnly = false;
    this->hasLimit = false;
    this->defKickMsg = "Bye, miss you";
    this->currentCount = 0;
    operators[creator.getNick()] = &creator;
    UserMessage("JOIN", name + " * :me\n", creator);
    whoIsChan(creator);
    // ServerMessage(RPL_TOPIC, ":no topic\n", creator);
    // ServerMessage(RPL_NAMREPLY, "@ " + name + " :@" + creator.getNick() + "\n", creator);
    // ServerMessage(RPL_ENDOFNAMES, name + " :END of /NAMES list\n", creator);
}

Channel::~Channel() {

}
//
// std::string Channel::getName( void ) {
//     return name;
// }
//

// std::string Channel::getPass( void ) {
//     return pass;
// }

// size_t Channel::getLimit( void ) {
//     return limit;
// }

void Channel::addMember( Client &add ) {
    if (!isInChan(add.getNick())) {
        std::map<std::string, Client *> sent;
        members[add.getNick()] = &add;
        UserMessage("JOIN", name + " * :welcome\n", add);
        whoIsChan(add);
        echoToAll(add, "JOIN", "", true, sent);
        this->currentCount++;
    }
}

void Channel::echoToAll(Client &client, std::string cmd, std::string trailing, bool chan, std::map<std::string, Client *> &sent) {
    std::string client_info = USER_FN(client.getNick(), client.getUserName(), client.getHostName());
    std::string message = ":" + client_info + " " + cmd;

    if (chan)
        message += " " + name;
    if (!trailing.empty())
        message += " " + trailing;
    message += "\n";
    for (iter it = members.begin(); it != members.end(); it++) {
        if (it->second->getNick() != client.getNick() && sent.find(it->second->getNick()) == sent.end()) {
            send(it->second->getFD(), message.data(), message.length(), 0);
            sent[it->second->getNick()] = it->second;
        }
    }
    for (iter it = operators.begin(); it != operators.end(); it++) {
        if (it->second->getNick() != client.getNick() && sent.find(it->second->getNick()) == sent.end()) {
            send(it->second->getFD(), message.data(), message.length(), 0);
            sent[it->second->getNick()] = it->second;
        }
    }
}

// void Channel::makeChanOp( Client &, Client & ) {
//
// }
//

// void Channel::invite( Client &, Client & ) {
//
// }
//

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
//
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
    
    if (mems != members.end()) {
        members.erase(old_nick);
        members[new_nick] = &client;
    }
    if (oper != operators.end()) {
        operators.erase(old_nick);
        operators[new_nick] = &client;
    }
}

unsigned int Channel::getCurrentCount(void) const
{
    return this->currentCount;
}