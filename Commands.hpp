#pragma once

#include "Functions.hpp"

class Commands : public Functions {
    public:
		void CAP( void );
		void PASS( void );
        void NICK( void );
		void USER( void );
		void PING( void );
		void OPER(void);
		void QUIT( void );
		void JOIN( void );
		void PART( void );
		void TOPIC(void);
		void INVITE(void);
		void KICK(void);
		void MOTD( void );
		void MODE( void );
		void PRIVMSG( void );
		void NOTICE( void );
		void WHOIS( void );
		void KILL (void);
};