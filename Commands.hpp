#pragma once

#include "Functions.hpp"

class Commands : public Functions {
    public:
		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CONNECTION MESSAGES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		void CAP( void );
		void PASS( void );
        void NICK( void );
		void USER( void );
		void PING( void );
		void OPER(void);
		void QUIT( void );

		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CHANNEL OPERATIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		void JOIN( void );
		void PART( void );
		void TOPIC(void);
		void INVITE(void);
		void KICK(void);

		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SERVER QUERIES / COMMANDS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		void MOTD( void );
		void MODE( void );

		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SENDING MESSAGES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		void PRIVMSG( void );
		void NOTICE( void );

		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ USER-BASED QUERIES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		void WHOIS( void );

		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ OPERATOR MESSAGES ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		void KILL (void);
};