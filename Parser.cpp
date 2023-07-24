#include "Parser.hpp"

Parser::Parser() {
	func[std::string("NICK")] = &Parser::NICK;
	func[std::string("CAP")] = &Parser::CAP;
	func[std::string("JOIN")] = &Parser::JOIN;
	func[std::string("USER")] = &Parser::USER;
	func[std::string("MODE")] = &Parser::MODE;
	func[std::string("PING")] = &Parser::PING;
	func[std::string("PART")] = &Parser::PART;
	func[std::string("PRIVMSG")] = &Parser::PRIVMSG;
	func[std::string("PASS")] = &Parser::PASS;
	func[std::string("MOTD")] = &Parser::MOTD;
	func[std::string("QUIT")] = &Parser::QUIT;
}

Parser::~Parser() {}

void Parser::takeInput( std::string Input, int fd, Client client ) {
	std::map<std::string, Funcs>::iterator command;
	std::map<int, Client>::iterator origin;

	this->input = Input;
	this->fd = fd;
	this->findCmdArgs();

	origin = clients.find(fd);
	if (origin == clients.end())
		clients[fd] = client;
	while (!multi_cmd.empty() && !multi_cmd.front().empty()) {
		cmd = multi_cmd.front().front();
		std::transform(cmd.begin(), cmd.end(), cmd.begin(), toupper);
		multi_cmd.front().pop_front();
		args = multi_cmd.front();
		command = func.find(cmd);
		if (command != func.end())
			(this->*command->second)();
		else
			ServerMessage(ERR_UNKNOWNCOMMAND, ":command not found\n");
		multi_cmd.pop_front();
	}
}

void Parser::findCmdArgs( void ) {
	std::string::size_type pos;
	devector<std::string> tmp;

	args.clear();
	while (!input.empty()) {
		if (input[0] == ':') {
			pos = input.find_first_of("\r\n");
			args.push_back(input.substr(0, pos));
			input.erase(0, pos + 1);
		} else if ((pos = input.find_first_of(" \r\n")) != input.npos) {
			args.push_back(input.substr(0, pos));
			input.erase(0, pos + 1);
		}
		std::cout << args.back() << std::endl;
	}
	while (!args.empty()) {
		tmp.clear();
		args.pop_back();
		while (!args.empty() && !args.back().empty() ) {
			tmp.push_front(args.back());
			args.pop_back();
		}
		multi_cmd.push_front(tmp);
	}
	std::cout << multi_cmd.size() << std::endl;
	for (size_t i = 0; i < multi_cmd.size(); i++) {
		for (size_t j = 0; j < multi_cmd[i].size(); j++) {
			std::cout << "|" << multi_cmd[i][j] << "|" << std::endl;
		}
		std::cout << std::endl;
	}
}
