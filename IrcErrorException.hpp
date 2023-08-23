#pragma once

#include <exception>

class IrcErrorException : public std::exception {
	private:
		const char *what_arg;
	public:
		IrcErrorException(const char *what_arg);
		const char * what ( ) const throw ();
};