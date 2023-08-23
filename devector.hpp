#pragma once

#include <vector>

template < class T >
class devector : public std::vector<T> {
	public:
		void pop_front( void ) {
			this->erase(this->begin());
		}
		void push_front( const T value ) {
			this->insert(this->begin(), value);
		}
};