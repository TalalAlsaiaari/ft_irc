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
}

Parser::~Parser() {}

void Parser::takeInput( std::string Input, int fd, Client client ) {
	this->input = Input;
	this->fd = fd;
	this->findCmd();
	try {
		clients.at(fd);
	} catch (std::exception &e) {
		clients[fd] = client;
	} try {
		func.at(cmd);
		((*this).*(func[cmd]))();
	} catch (std::exception &e) {
		// cmd not found
		std::cout << "cmd not found\n";
	}
}

void Parser::findCmd( ) {
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
	args.pop_front();
	std::cout << "|" << cmd << "|" << std::endl;
	for (size_t i = 0; i < args.size(); i++)
		std::cout << "args[i] " << args[i] << std::endl;
}

std::string Parser::getCmd( void ) {
	return cmd;
}
