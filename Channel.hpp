#pragma once

#include <map>
#include "Client.hpp"

class Channel {
	private:
		std::map<std::string, Client> members;
		std::map<std::string, Client> invited;
		std::map<std::string, Client> operators;
		std::map<std::string, Client> banned;
		std::string name;
		std::string pass;
		size_t limit;
		bool i;
		bool k;
		bool l;
		bool t;
		bool o;
		bool l;
	public:
		Channel();
		Channel( std::string name, Client creator );
		~Channel();
		void echotoAll( std::string message , Client origin);
}