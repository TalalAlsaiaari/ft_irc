#pragma once

#include "Client.hpp"
#include <sys/socket.h>

void ConnectionMessage( Client &client);
void ServerMessage(std::string error, std::string message, Client &client);
void UserMessage(std::string cmd, std::string message, Client &client);
void UsertoUser(Client origin, Client dest, std::string cmd, std::string mess);
// void quitMsg(Client, std::string);
// void killMsg(Client, Client);
// void errMsg(std::map<std::string, Client>::iterator, std::string);
