#pragma once

#include "Client.hpp"
#include "Functions.hpp"
#include <map>

class Channel {
private:
// A client_iterator synonym is created to make the code easier to read	
	typedef std::map<std::string, Client *>::iterator client_iterator;

	std::string name; //name of the channel
	std::string pass; //password to join the channel
	Client*	operator; //A pointer to the client who is the operator of the channel
	size_t limit; //The limit on the number of members allowed in the channel
	bool i; // A boolean flag indicating whether the client is invited into the channel
/* maps as private class members will be initialized
with null by the Constructor when the Class is created */
	std::map<std::string, Client *> members;
	std::map<std::string, Client *> invited;
	std::map<std::string, Client *> operators;
	std::map<std::string, Client *> banned;
//	bool k;
	bool l;
//	bool t;
//	bool o;

	Channel();

public:
	Channel( std::string name, std::string pass, Client* operator );
	~Channel();

/* Getters */

    std::string                 	getName() const;
    std::string                 	getPass() const;
    size_t                      	getLimit() const;
    size_t                      	getSize() const;

    std::map<std::string, Client>	getMembers() const;
	std::map<std::string, Client>	getInvited() const;
    std::map<std::string, Client>	getOperators() const;
	std::map<std::string, Client>	getBanned() const;


/* Setters */
//if you want to change name, password or limits
	void							setName(std::string name);
	void							setPass(std::string pass);
	void							setLimit(size_t limit);


/* Channel Methods */
//sends a message to all clients
    void                        broadcast(const std::string& message);
//sedns a message to all clients except a specific client(s)
    void                        broadcast(const std::string& message, Client* exclude);
//adds client to the channel
    void                        addMember(Client* client);
//removes client from the channel
    void                        removeMember(Client* client);
//kicks the client from the channel and delete 
    void                        kick(Client* client, Client* target, const std::string& reason);
		
//	void echotoAll( std::string message , Client origin);
};
