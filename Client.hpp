#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <vector>
#include <string>


class Client {
	private:
		int 		fd;
		std::string nick;
		std::string real_name;
		std::string user_name;
		std::string server_name;
		std::string host_name;
		bool registered;
		bool pass_registered;
	public:
		Client( );
		Client( int fd );
		Client( int fd , std::string host_name );
		~Client( );
		void setNick( std::string nick );
		void setRealName( std::string real_name );
		void setHostName( std::string host_name );
		void setUserName( std::string user_name );
		void setServerName( std::string server_name );
		std::string getNick( void );
		std::string getRealName( void );
		std::string getHostName( void );
		std::string getUserName( void );
		std::string getServerName( void );
		int getFD( void );
		bool isRegistered( void );
		void registration( void );
		bool isPassGood( void );
		void passGood( void );
};

#endif