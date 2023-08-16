#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <map>
#include <stdexcept>
#include "devector.hpp"

class Client {
	private:
		int			fd;
		std::string	nick;
		std::string	real_name;
		std::string	user_name;
		std::string	server_name;
		std::string	host_name;
		std::string	recv_buff;
		devector<std::string> send_buff;
		bool		registered;
		bool		pass_registered;
		bool		is_serverOp;
		bool		is_invisible;
	public:
		Client( );
		Client( int fd );
		Client( int fd, std::string host_name );
		~Client( );
		Client &operator=(const Client &other);
		void setNick( std::string nick );
		void setRealName( std::string real_name );
		void setHostName( std::string host_name );
		void setUserName( std::string user_name );
		void setServerName( std::string server_name );
		void setBuff( std::string buff );
		void pushSendBuff( std::string send );
		std::string getNick( void );
		std::string getRealName( void );
		std::string getHostName( void );
		std::string getUserName( void );
		std::string getServerName( void );
		std::string &getBuff( void );
		devector<std::string> &getSendBuff( void);
		int getFD( void );
		bool isRegistered( void );
		void registration( void );
		bool isPassGood( void );
		void passGood( void );
		bool isInvisibile( void );
		void setInvisibility( bool invis );
		bool isServerOp( void );
		void setServerOp( bool oper );
};

#endif