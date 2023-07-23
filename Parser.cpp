#include "Parser.hpp"

Parser::Parser() {
	func[std::string("nick")] = &Parser::NICK;
	func[std::string("cap")] = &Parser::CAP;
	func[std::string("join")] = &Parser::JOIN;
	func[std::string("user")] = &Parser::USER;
	func[std::string("mode")] = &Parser::MODE;
	func[std::string("ping")] = &Parser::PING;
	func[std::string("part")] = &Parser::PART;
	func[std::string("privmsg")] = &Parser::PRIVMSG;
	func[std::string("pass")] = &Parser::PASS;
	func[std::string("motd")] = &Parser::MOTD;
}

Parser::~Parser() {}

void Parser::takeInput( std::string Input, int fd, Client client ) {
	std::map<std::string, Funcs>::iterator command;
	std::map<int, Client>::iterator origin;

	this->input = Input;
	this->fd = fd;
	this->findCmd();

	origin = clients.find(fd);
	if (origin == clients.end())
		clients[fd] = client;

	command = func.find(cmd);
	if (command != func.end())
		(this->*command->second)();
	else
		ServerMessage(ERR_UNKNOWNCOMMAND, ":command not found\n");
}

void Parser::findCmd( void ) {
	args.clear();
	int x = 0;

	while (!input.empty()) {
		std::string::size_type pos1 = input.find_first_of("\r\n");
		std::string::size_type pos2 = input.find_first_of(" ");
		if (input[0] == ':') {
			args.push_back(input);
			break;
		} else if (pos1 < pos2) {
			std::cout << 1 << std::endl;
			args.push_back(input.substr(0, pos1));
			input.erase(0, pos1 + 1);
		} else if (pos2 == input.npos) {
			std::cout << 2 << std::endl;
			args.push_back(input);
			input.erase(0, input.length());
		} else {
			std::cout << 3 << std::endl;
			args.push_back(input.substr(0, pos2));
			input.erase(0, pos2 + 1);
		}
		std::cout << args[x++] << std::endl;
	}
	cmd = args.front();
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), tolower);
	args.pop_front();
	std::cout << "|" << cmd << "|" << std::endl;
	for (size_t i = 0; i < args.size(); i++)
		std::cout << "args[i] " << args[i] << std::endl;
}

std::string Parser::getCmd( void ) {
	return cmd;
}
