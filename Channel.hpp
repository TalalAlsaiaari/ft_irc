#pragma once

#include <map>
#include <string>
#include "Client.hpp"

class Channel
{
	private:
		std::map<std::string, Client *> members;
		std::map<std::string, Client *> invited;
		std::map<std::string, Client *> operators;
		std::string		name;
		std::string 	pass;
		std::string		topic;
		size_t			limit;
		bool			inviteOnly;
		bool			l;
		bool			k;
		bool			t;
	public:
		Channel();
		Channel( std::string, std::string, Client & );
		~Channel();
		std::string getName( void );
		std::string getTopic( void );
		std::string getPass( void );
		size_t getLimit( void );
		void addMember( Client & );
		void makeChanOp( Client &, Client & );
		void invite( Client &, Client & );
		void removeMember( Client & );
		void setTopic( std::string, Client & );
};
