#include "Client.hpp"


Client::Client() : registered(false), pass_registered(false), is_operator(false), is_invisible(false) {}

Client::Client( int fd ) : fd(fd), registered(false), pass_registered(false), is_operator(false), is_invisible(false) {}

Client::Client( int fd, std::string host_name ) : fd(fd), host_name(host_name), registered(false), pass_registered(false), is_operator(false), is_invisible(false) {}

Client &Client::operator=(const Client &other) {
	this->fd = other.fd;
	this->nick = other.nick;
	this->real_name = other.real_name;
	this->user_name = other.user_name;
	this->server_name = other.server_name;
	this->host_name = other.host_name;
	this->registered = other.registered;
	this->pass_registered = other.pass_registered;
	this->is_invisible = other.is_invisible;
	this->is_operator = other.is_operator;
	return *this;
}

Client::~Client( ) {}

void Client::setNick( std::string nick ) {
	this->nick = nick;
}

void Client::setBuff( std::string buff ) {
	this->buff = buff;
}

void Client::setRealName( std::string real_name ) {
	if (!real_name.empty() && real_name[0] == ':')
		real_name.erase(0, 1);
	this->real_name = real_name;
}

void Client::setHostName( std::string host_name ) {
	this->host_name = host_name;
}

void Client::setUserName( std::string user_name ) {
	this->user_name = user_name;
}

void Client::setServerName( std::string server_name ) {
	this->server_name = server_name;
}

std::string Client::getNick( void ) {
	return nick;
}

std::string Client::getRealName( void ) {
	return real_name;
}

std::string Client::getHostName( void ) {
	return host_name;
}

std::string Client::getUserName( void ) {
	return user_name;
}

std::string Client::getServerName( void ) {
	return server_name;
}

std::string &Client::getBuff( void ) {
	return buff;
}

int Client::getFD( void ) {
	return fd;
}

bool Client::isRegistered( void ) {
	return registered;
}

void Client::registration( void ) {
	registered = true;
}

bool Client::isPassGood( void ) {
	return pass_registered;
}

void Client::passGood( void ) {
	pass_registered = true;
}

bool Client::isInvisibile( void ) {
	return is_invisible;
}

void Client::setInvisibility( bool invis ) {
	is_invisible = invis;
}

bool Client::isOperator( void ) {
	return is_operator;
}

void Client::setOperator( bool oper ) {
	is_operator = oper;
}