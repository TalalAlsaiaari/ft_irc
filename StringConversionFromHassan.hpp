#pragma once

#include <sstream>
#include <string>

template < typename T >
void StrtoAny(std::string str, T &value) {
	std::stringstream ss(str);

	ss >> value;
}

template < typename T >

std::string AnytoStr(T value) {
	std::stringstream ss;

	ss << value;
	return ss.c_str();
}