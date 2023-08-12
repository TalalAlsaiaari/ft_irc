#include "Channel.hpp"

Channel::Channel() {
}

Channel::Channel( std::string name, std::string pass, Client &creator ) {
    this->name = name;
    this->pass = pass;
    operators[creator.getNick()] = &creator;
}

Channel::~Channel() {

}

std::string Channel::getName( void ) {
    return name;
}

std::string Channel::getTopic( void ) {
    return topic;
}

std::string Channel::getPass( void ) {
    return pass;
}

size_t Channel::getLimit( void ) {
    return limit;
}

void Channel::addMember( Client &add ) {

    if (members.find(add.getNick()) == members.end())
        members[add.getNick()] = &add;
    else if (operators.find(add.getNick()) == operators.end())
        members[add.getNick()] = &add;
    else
        std::cout << "already in channel\n";
}

void Channel::makeChanOp( Client &, Client & ) {

}

void Channel::invite( Client &, Client & ) {

}

void Channel::removeMember( Client & ) {

}

void Channel::setTopic( std::string, Client & ) {

}

bool Channel::isInChan( std::string Nick ) {
    std::map<std::string, Client *>::iterator member;
    std::map<std::string, Client *>::iterator oper;

    member = members.find(Nick);
    oper = operators.find(Nick);
    if (member == members.end() && oper == operators.end())
        return false;
    return true;
}

bool Channel::isInvited( std::string Nick ) {
    std::map<std::string, Client *>::iterator invite;

    invite = invited.find(Nick);
    if (invite == invited.end())
        return false;
    return true;
}
