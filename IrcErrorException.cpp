#include "IrcErrorException.hpp"

IrcErrorException::IrcErrorException(const char *what_arg) : what_arg(what_arg) {}

const char * IrcErrorException::what ( ) const throw () {
	return what_arg;
}