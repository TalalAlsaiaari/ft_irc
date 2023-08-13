#pragma once

#include <map>
#include <string>
#include "Client.hpp"

class Channel {
	private:
		std::map<std::string, Client *> members;
		std::map<std::string, Client *> operators;
		std::map<std::string, Client *> invited;
	public:
		Channel(Client &creator)
}

