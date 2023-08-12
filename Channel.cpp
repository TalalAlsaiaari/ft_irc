#include "Channel.hpp"

Channel::Channel() {
}

Channel::Channel( std::string, std::string, Client & ) {

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
    std::map<std::string, Client *>::iterator new_mem;

    new_mem = members.find(add.getNick());
    if (new_mem == members.end())
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
