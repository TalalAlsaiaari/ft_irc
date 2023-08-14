#pragma once

#include <map>
#include <string>
#include "Client.hpp"
#include "Messages.hpp"
#include "ErrorCodes.hpp"

class Channel
{
	private:
		typedef std::map<std::string, Client *>::iterator iter;
		std::map<std::string, Client *> members;
		std::map<std::string, Client *> invited;
		std::map<std::string, Client *> operators;
		std::string		name;
		// std::string 	pass;
		std::string		topic;
		size_t			limit;
		bool			inviteOnly;
		bool			hasLimit;
		std::string		defKickMsg;
		unsigned int	currentCount;
	public:
		Channel();
		Channel( std::string, Client & );
		~Channel();
		// std::string getName( void );
		// std::string getPass( void );
		// size_t getLimit( void );
		void addMember( Client & );
		void echoToAll(Client &client, std::string cmd, std::string trailng, bool chan, std::map<std::string, Client *>&sent );
		// void makeChanOp( Client &, Client & );
		// void invite( Client &, Client & );
		void removeMember( Client & );
		void updateMemberNick( Client &, std::string, std::string );
		void whoIsChan( Client & );
		bool isInChan( std::string );
		bool isInvited( std::string );
		void setTopic(std::string topic);
		bool hasTopic(void);
		std::string const getTopic(void) const;
		unsigned int getCurrentCount(void) const;
};
