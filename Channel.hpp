#pragma once

#include "Functions.hpp"
#include <map>

class Client;

class Channel 
{
	// A client_iterator synonym is created to make the code easier to read	
	typedef std::map<std::string, Client>::iterator client_iterator;

private:
	/* maps as private class members will be initialized
	with null by the Constructor when the Class is created */
	std::map<std::string, Client *> members;
	std::map<std::string, Client *> invited;
	std::map<std::string, Client *> operators;
	std::map<std::string, Client *> banned;
	
	std::string		name; //name of the channel
	std::string 	pass; //password to join the channel
	std::string		topic; // topic of the channel
	Client*			owner; //A pointer to the client who is the owner of the channel
	size_t 			limit; //The limit on the number of members allowed in the channel
	bool 			inviteOnly; // A Boolean flag indicating whether the channel is by invitation or not
	// bool l;
	// bool o;
	// bool k;
	// bool t;

	Channel();

public:
	Channel( std::string name, Client* owner, std::string pass );
	~Channel();


/* Getters */

    std::string		getName() const;
    std::string		getPass() const;
	std::string		getTopic() const;
	std::string		getOwner() const;
    size_t			getLimit() const;
    size_t			getSize() const;

	// Get a maps of all channel members, as well as a list of invited clients, 
	// all operators and banned clients
    const std::map<std::string, Client>&	getMembers() const;
	const std::map<std::string, Client>&	getInvited() const;
    const std::map<std::string, Client>&	getOperators() const;
	const std::map<std::string, Client>&	getBanned() const;


/* Setters */
//if you want to change name, password, topic or limits
	void		setName(std::string nameValue);
	void		setPass(std::string passValue);
	void		setTopic(std::string topicValue);
	void		setLimit(size_t limitValue);


/* Channel Methods */

	bool		isInviteOnly() const;
	bool 		checkPassword(const std::string& password) const;
	bool        isMember(Client* client) const;
	bool        isOperator(Client* client) const;
	bool		isInvited(Client* client) const;
	bool 		isEmpty() const;

	// Add the corresponding clients to the maps we need
	void 		addMember(Client* client);
	void 		addInvited(Client* client);
	void 		addOperator(Client* client, Client* invoker);
	void 		addBanned(Client* client, Client* invoker, const std::string& reason);

	// Remove the corresponding clients from the map we need
	void 		removeMember(Client* client);
	void 		removeOperator(Client* client);
	void 		removeInvited(Client* client);
	void 		removeBanned(Client* client);

	//sends a message to all clients
    void		broadcast(const std::string& message);
	//sedns a message to all clients except a specific client(s)
    void		broadcast(const std::string& message, Client* exclude);
    void		kick(Client* client, Client* target, const std::string& reason);
		
};
