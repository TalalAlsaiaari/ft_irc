#pragma once

#include "Functions.hpp"

class Commands : public Functions {
    public:
        void NICK( void );
		void CAP( void );
		void JOIN( void );
		void USER( void );
		void MODE( void );
		void PING( void );
		void PART( void );
		void PRIVMSG( void );
		void NOTICE( void );
		void PASS( void );
		void MOTD( void );
		void QUIT( void );
		void WHOIS( void );
		void OPER(void);
		void KILL (void);
		void TOPIC(void);
};