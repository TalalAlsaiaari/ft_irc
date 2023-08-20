#pragma once

#include <map>
#include <string>
#include <sstream>
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
		std::string 	pass;
		std::string		topic;
		size_t			limit;
		bool			inviteOnly;
		bool			hasLimit;
		bool			hasKey;
		std::string		defKickMsg;
		unsigned int	currentCount;
		std::string		modes;
		
	public:
		Channel();
		Channel( std::string, Client & );
		~Channel();
		void addMember( Client &add , std::string key);
		std::string echoToAll(Client &client, std::string cmd, std::string trailng, bool chan, std::map<std::string, Client *>&sent );
		void makeChanOp( Client &, Client & );
		void unsetChanOp(Client &src, Client &dst);
		void removeMember( Client & );
		void updateMemberNick( Client &, std::string, std::string );
		void whoIsChan( Client & );
		void addInvite( std::string nick, Client &client );
		void removeInvite( std::string nick );
		void setTopic(std::string topic);
		void setChannelOp(Client& target);
		void setInviteOnly( bool invite );
		void setModes(char mode);
		void removeModes(char mode);
		unsigned int getCurrentCount(void) const;

		void chanModes(char mode, char sign, devector<std::string> &arguments, Client &current);
		void modeI(char mode, char sign, Client &current);
		void modeO(char sign, devector<std::string> &args, Client &current);
		void modeK(char sign, devector<std::string> &args, Client &current);
		void modeL(char sign, devector<std::string> &args, Client &current);
		// void modeT(char mode, char sign);

		std::string const getTopic(void) const;
		std::string const getDefKickMsg(void) const;
		std::string const getModes(void) const;

		bool isInviteOnly( void );
		bool hasTopic(void);
		bool isInChan( std::string );
		bool isInvited( std::string );
		bool checkEntrance( std::string nick, Client &client, std::string key );
		bool isUserOp(std::string chanName, Client &user);
		void modeMsg(Client &src, Client &dst, std::string mode);
		// bool isCorrectKey(std::string chanName, Client &user, std)
};
