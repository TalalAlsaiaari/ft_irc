#include "Messages.hpp"
#include "ErrorCodes.hpp"

// void ConnectionMessage( Client &client) {
// 	ServerMessage(RPL_WELCOME, " :Welcome You are now known as " + USER_FN(client.getNick(), client.getUserName(), client.getHostName()) + "\n" );
// 	ServerMessage(RPL_YOURHOST, " :Your host is " + client.getHostName() + "\n");
// 	ServerMessage(RPL_CREATED, " :This server was created some time recently\n");
// 	ServerMessage(RPL_MYINFO, client.getHostName() + "\n");
// 	ServerMessage(RPL_ISUPPORT, "MODES=2 MAXNICKLEN=16 NICKLEN=16 CHANNELLEN=50 CHANTYPES=# :are supported by this server\n");
// }

void ServerMessage(std::string error, std::string message, Client &client) {
	std::string mes = ":" + client.getServerName() + error + client.getNick() + " " + message;
	send(client.getFD(), &mes[0], mes.length(), 0);
}

void UserMessage(std::string cmd, std::string message, Client &client) {
	std::string mes = ":" + USER_FN(client.getNick(), client.getUserName(), client.getHostName()) + " " + cmd + " " + message;
	send(client.getFD(), &mes[0], mes.length(), 0);
}

void UsertoUser(Client origin, Client dest, std::string cmd, std::string mess) {
	std::string message = ":" + USER_FN(origin.getNick(), origin.getUserName(), origin.getHostName());
	message += " " + cmd + " " + origin.getNick() + " ";
	message += ":";
	message += mess + "\n";
	std::cout << message << std::endl;
	send(dest.getFD(), &message[0], message.length(), 0);
}