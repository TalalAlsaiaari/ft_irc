#include "Client.hpp"


Client::Client() : registered(false) {}

Client::Client( int fd ) : fd(fd), registered(false) {}

Client::~Client( ) {}

void Client::setNick( std::string nick ) {
	this->nick = nick;
}

void Client::setRealName( std::string real_name ) {
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

int Client::getFD( void ) {
	return fd;
}

bool Client::isRegistered( void ) {
	return registered;
}

void Client::registration( void ) {
	registered = true;
}