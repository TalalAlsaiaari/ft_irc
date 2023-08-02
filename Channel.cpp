#include "Chsnnel.hpp"


/* Constructor and Destructor */

Channel( std::string nameValue, std::string passValue, Client *operatorValue ) 
    : name(nameValue), pass(passValue), operator(operatorValue), limit(0) {}

Channel::~Channel() {}


/* Getters */

std::string                 Channel::getName() const { return name; }
std::string                 Channel::getPass() const { return pass; }
Client*                     Channel::getOperator() const { return operator; }
size_t                      Channel::getLimit() const { return limit; }
size_t                      Channel::getSize()const { return members.size(); }

std::map<std::string, Client>    Channel::getMembers()
{
    client_iterator it_b = members.begin();
    client_iterator it_e = members.end();

    while (it_b != it_e)
    {
        const std::string& nickname = (client == operator ? "@" : "") + it_b->first;
        const Client& client = it_b->second;

        // Добавляем в новую карту members
        members[nickname] = client;
        
        it_b++;
    }

    return members;
}


/* Setters */

void                        Channel::setName(std::string nameValue) { name = nameValue; }
void                        Channel::setPass(std::string passValue) { pass = passValue; }
void                        Channel::set_limit(size_t limitValue) { limit = limitValue; }


/* Channel Methods */

void                        Channel::broadcast(const std::string& message)
{
    client_iterator it_b = _clients.begin();
    client_iterator it_e = _clients.end();

    while (it_b != it_e)
    {
        (*it_b)->write(message);
        it_b++;
    }
}

void                        Channel::broadcast(const std::string& message, Client* exclude)
{
    client_iterator it_b = _clients.begin();
    client_iterator it_e = _clients.end();

    while (it_b != it_e)
    {
        if (*it_b == exclude)
        {
            it_b++;
            continue;
        }

        (*it_b)->write(message);
        it_b++;
    }
}

void                        Channel::addMember(Client* client)
{
    std::string nick = clien->getNick();
    members[nick] = client;
}

void                        Channel::removeMember(Client* client)
{
    client_iterator it_b = _clients.begin();
    client_iterator it_e = _clients.end();

    while (it_b != it_e)
    {
        if (*it_b == client)
            _clients.erase(it_b);
        
        it_b++;
    }

    client->set_channel(NULL);

    if (client == _admin)
    {
        _admin = *(_clients.begin());

        std::string message = client->get_nickname() + " is now the admin of the channel " + _name;
        log(message);
    }
}

void                        Channel::kick(Client* client, Client* target, const std::string& reason)
{
    this->broadcast(RPL_KICK(client->get_prefix(), _name, target->get_nickname(), reason));
    this->remove_client(target);

    std::string message = client->get_nickname() + " kicked " + target->get_nickname() + " from channel " + _name;
    log(message);
}
