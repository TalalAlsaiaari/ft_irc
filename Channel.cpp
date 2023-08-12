#include "Channel.hpp"
#include "Client.hpp"


/* Constructor and Destructor */

Channel( std::string nameValue, Client *ownerValue, std::string passValue, bool inviteOnlyValue == false ) 
    : name(nameValue), owner(ownerValue), pass(passValue), topic(""), limit(0), inviteOnly(inviteOnlyValue) {}

Channel::~Channel() {}


/* Getters */

std::string     Channel::getName() const { return name; }
std::string     Channel::getPass() const { return pass; }
std::string     Channel::getTopic() const { return topic; }
Client*         Channel::getOwner() const { return owner; }
size_t          Channel::getLimit() const { return limit; }
size_t          Channel::getSize()const { return members.size(); }

const std::map<std::string, Client>&    Channel::getMembers() const { return members; }
const std::map<std::string, Client>&    Channel::getInvited() const { return invited; }
const std::map<std::string, Client>&    Channel::getOperators() const { return operators; }
const std::map<std::string, Client>&    Channel::getBanned() const { return banned; }


/* Setters */

void        Channel::setName(std::string nameValue) { name = nameValue; }
void        Channel::setPass(std::string passValue) { pass = passValue; }
void        Channel::setTopic(std::string topic) { pass = topicValue; }
void        Channel::setLimit(size_t limitValue) { limit = limitValue; }


/* Channel Methods */

// Whether the channel is an invitation-only channel
bool        Channel::isInviteOnly() const { return inviteOnly; }

// Compare the provided password with the channel password
bool        Channel::checkPassword(const std::string& password) const { return password == channelPassword; }

// Whether the client is in the list of channel members, operators, invited channel members
bool        Channel::isMember(Client* client) const { return members.find(client->getNick()) != members.end(); }
bool        Channel::isOperator(Client* client) const { return operators.find(client->getNick()) != operators.end(); }
bool        Channel::isInvited(Client* client) const { return invited.find(client->getNick()) != invited.end(); }

// Whether the channel is empty
bool        Channel::isEmpty() const { return members.empty() && operators.empty(); }

// Adding a client to the member map if it is not already there
void        Channel::addMember(Client* client) 
{
    if (inviteOnly)
    { // If the channel is invitation-only
        if (invited.find(client->getNick()) == invited.end())
        {
            // The client is not invited, send a message
            std::string message = "NOTICE " + name + " :" + client->getNick() + " :You're not invited to join this channel";
            client->write(message);
            return;
        }
    }

    if (members.find(client->getNick()) == members.end()) 
    {
        members[client->getNick()] = client;
    }
}

// Adding a client to the invite map if he is not already there
void        Channel::addInvited(Client* client) 
{
    if (invited.find(client->getNick()) == invited.end()) 
    {
        invited[client->getNick()] = client;
    }
}

// Adding a client to the operator map if it is not already there
void        Channel::addOperator(Client* client, Client* invoker) 
{
    // Access rights check: owner or operators can add operators
    if (operators.find(invoker->getNick()) != operators.end() || owner == invoker) 
    {
        if (operators.find(client->getNick()) == operators.end()) 
        {
            operators[client->getNick()] = client;
        }
    }
}

// Adding a client to the banned map if it is not already there
void        Channel::addBanned(Client* client, Client* invoker, const std::string& reason) 
{
    // Checking access rights: owner or operators can ban customers
    if (operators.find(invoker->getNick()) != operators.end() || owner == invoker) 
    {
        if (banned.find(client->getNick()) == banned.end()) 
        {
            banned[client->getNick()] = client;

            // Sending ban notification to IRC chat according to RFC 2812 protocol
            std::string notice = "NOTICE " + name + " :" + client->getNick() + " has been banned by " + invoker->getNickname() + " (" + reason + ")";
            // Отправляем 'notice' всем клиентам в этом канале
            broadcast(notice);

            // Saving information about the ban in the logbook
            std::string logMessage = client->getNick() + " был забанен оператором " + invoker->getNickname() + " по причине: " + reason;
            log(logMessage);
        }
    }
}

// Removing a member from map members
void        Channel::removeMember(Client* client) 
{
    client_iterator it = members.find(client->getNick());
    if (it != members.end()) 
    {
        std::string message = "PART " + name + " :" + client->getNick();
        broadcast(message);
        members.erase(it);

        std::string logMessage = client->getNick() + " has left channel " + name;
        log(logMessage);
    }
    // Deleting a channel a client is a member of
    clientToRemove->removeChannelOfClient(name);
}

// Removing an operator from map operators
void        Channel::removeOperator(Client* client) 
{
    // Check if the client is the channel owner
    if (owner != client) {
        client->write("NOTICE " + name + " :You are not authorized to remove operators.");
        return;
    }
    
    client_iterator it = operators.find(client->getNick());
    if (it != operators.end()) 
    {
        std::string message = "MODE " + name + " -o " + client->getNick();
        broadcast(message);
        
        // Adding an operator to the ordinary membership map
        members[client->getNick()] = client;
        
        operators.erase(it);
        
        std::string logMessage = client->getNick() + " is no longer an operator in channel " + name;
        log(logMessage);
    }
}

// Removing an invitee from map invited
void        Channel::removeInvited(Client* client) 
{
    client_iterator it = invited.find(client->getNick());
    if (it != invited.end()) 
    {
        std::string message = "INVITE " + client->getNick() + " :" + name;
        client->write(message);
        
        invited.erase(it);

        std::string logMessage = client->getNick() + " is no longer invited to channel " + name;
        log(logMessage);
    }
}

// Removing a banned person from map banned (unbanning)
void        Channel::removeBanned(Client* client) 
{
    client_iterator it = banned.find(client->getNick());
    if (it != banned.end()) 
    {
        std::string message = "MODE " + name + " +b " + client->getNick();
        broadcast(message);
        
        banned.erase(it);

        std::string logMessage = client->getNick() + " has been unbanned from channel " + name;
        log(logMessage);
    }
}

void        Channel::broadcast(const std::string& message)
{
    // Send a message to all channel members
    client_iterator it = members.begin();
    while (it != members.end()) 
    {
        it->second->write(message);
        it++;
    }

    // Sending a message to the channel operators
    it = operators.begin();
    while (it != operators.end()) 
    {
        it->second->write(message);
        it++;
    }

    // Send a message to the owner of the channel
    if (owner != NULL) 
    {
        owner->write(message);
    }
}

void        Channel::broadcast(const std::string& message, Client* exclude)
{
    // Send the message to all members of the channel, excluding the specified client
    client_iterator it = members.begin();
    while (it != members.end()) 
    {
        if (it->second != exclude) 
        {
            it->second->write(message);
        }
        it++;
    }

    // Send a message to the channel operators, excluding the specified client
    it = operators.begin();
    while (it != operators.end()) 
    {
        if (it->second != exclude) 
        {
            it->second->write(message);
        }
        it++;
    }

    // Send a message to the channel owner, excluding the specified client
    if (owner != NULL && owner != exclude) 
    {
        owner->write(message);
    }
}

void        Channel::kick(Client* client, Client* target, const std::string& reason) 
{
    // Check if the client has kick privileges (owner or operator)
    if (operators.find(client->getNick()) != operators.end() || owner == client) 
    {
        // Check if the target is the channel owner
        if (target == owner) 
        {
            client->write("NOTICE " + name + " :You cannot kick the channel owner.");
            return;
        }

        // Search for the target in different maps
        client_iterator itMember = members.find(target->getNick());
        client_iterator itOperator = operators.find(target->getNick());
        client_iterator itInvited = invited.find(target->getNick());
        client_iterator itBanned = banned.find(target->getNick());

        // Perform kick if the target is found in any of the maps
        if (itMember != members.end() || itOperator != operators.end() ||
            itInvited != invited.end() || itBanned != banned.end()) 
        {

            // Send kick notification to IRC chat
            std::string kickMessage = "KICK " + name + " " + target->getNick() + " :" + reason;
            broadcast(kickMessage);

            // Remove the target from appropriate maps using existing functions
            if (itMember != members.end()) removeMember(target);
            if (itOperator != operators.end()) removeOperator(target);
            if (itInvited != invited.end()) removeInvited(target);
            if (itBanned != banned.end()) removeBanned(target);

            // Write a log entry for the kick
            std::string logMessage = target->getNick() + " was kicked from channel " + name + " by " + client->getNick() + " (" + reason + ")";
            log(logMessage);
        }
    }
}
