#include "Parser.hpp"

Parser::Parser() {
	func["CAP"] = &Parser::CAP;
	func["PASS"] = &Parser::PASS;
	func["NICK"] = &Parser::NICK;
	func["USER"] = &Parser::USER;
	func["PING"] = &Parser::PING;
	func["OPER"] = &Parser::OPER;
	func["QUIT"] = &Parser::QUIT;
	func["JOIN"] = &Parser::JOIN;
	func["PART"] = &Parser::PART;
	func["TOPIC"] = &Parser::TOPIC;
	func["INVITE"] = &Parser::INVITE;
	func["KICK"] = &Parser::KICK;
	func["MODE"] = &Parser::MODE;
	func["PRIVMSG"] = &Parser::PRIVMSG;
	func["NOTICE"] = &Parser::NOTICE;
	func["WHOIS"] = &Parser::WHOIS;
	func["KILL"] = &Parser::KILL;
	func["MOTD"] = &Parser::MOTD;
}

Parser::~Parser() {}

void Parser::takeInput( std::string Input, int fd, Client &client ) {
	this->input = Input;
	this->fd = fd;
	this->findCmdArgs();
	current_client = &client;
	try {
		this->excecuteCommand();
		nicks[current_client->getNick()] = current_client;
	} catch (std::exception &e) {
		std::cout << "Exception caught: " << e.what();
	}
}

void Parser::findPass( void ) {
	std::string::size_type pos = input.find_first_of("\r\n");
	if (pos != input.npos) {
		args.push_back(input.substr(0, pos));
		input.erase(0, pos + 2);
		multi_cmd.push_back(args);
		args.clear();
	}
}

void Parser::findCmdArgs( void ) {
	std::string::size_type pos_nl;
	std::string::size_type pos_ws;

	args.clear();
	multi_cmd.clear();
	while (!input.empty()) {
		pos_ws = input.find_first_of(" ");
		pos_nl = input.find_first_of("\n\r");
		if (input[0] == ':' && pos_nl != input.npos) {
			args.push_back(input.substr(1, pos_nl - 1));
			input.erase(0, pos_nl + 2);
			multi_cmd.push_back(args);
			args.clear();
		} else if (pos_ws < pos_nl) {
			args.push_back(input.substr(0, pos_ws));
			pos_ws = input.find_first_not_of(" ", pos_ws);
			input.erase(0, pos_ws);
		} else if (pos_nl < pos_ws) {
			args.push_back(input.substr(0, pos_nl));
			pos_nl = input.find_first_not_of("\n\r", pos_nl);
			input.erase(0, pos_nl);
			multi_cmd.push_back(args);
			args.clear();
		}
		if (!args.empty() && makeUpper(args.front()) == "PASS")
			this->findPass();
	}
	for (size_t i = 0; i < multi_cmd.size(); i++) {
		for (size_t j = 0; j < multi_cmd[i].size(); j++) {
			std::cout << "|" << multi_cmd[i][j] << "|" << std::endl;
		}
		std::cout << std::endl;
	}
}

std::string Parser::makeUpper( std::string str) {
	std::string dest = str;
	std::transform(str.begin(), str.end(), dest.begin(), toupper);
	return dest;
}

void Parser::excecuteCommand( void ) {
	std::map<std::string, Funcs>::iterator command;

	while (!multi_cmd.empty() && !multi_cmd.front().empty()) {
		cmd = makeUpper(multi_cmd.front().front());
		multi_cmd.front().pop_front();
		args = multi_cmd.front();
		multi_cmd.pop_front();
		command = func.find(cmd);
		if (command != func.end()) {
			if (checkRegistration())
				(this->*command->second)();
			else
				ServerMessage(ERR_NOTREGISTERED, " :need to register first\n", *current_client);
		}
		else
			ServerMessage(ERR_UNKNOWNCOMMAND, " :command not found\n", *current_client);
	}
}

bool Parser::checkRegistration( void ) {
	if (cmd != "PASS" && cmd != "CAP" && cmd !="USER" && cmd != "NICK" && cmd != "QUIT") {
		if (current_client->isPassGood() && current_client->isRegistered())
			return true;
		return false;
	}
	return true;
}

void Parser::removeClient( std::string remove ) {
	client_it client = nicks.find(remove);
	
	if (client != nicks.end())
		nicks.erase(client);
}
