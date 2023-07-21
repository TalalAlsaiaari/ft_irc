#pragma once

#include "ErrorCodes.hpp"

#include <string>

class Error {
	private:
		int code;
		std::string message;
		void setCode( int code );
		void setMessage( std::string message );
	public:
		Error();
		~Error();
		void sendError( void );
};
