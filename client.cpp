#include "client.hpp"

Client::Client() : nick("nick"), real_name("Unknown"), user_name("user") {}

Client::Client( int fd ) : fd(fd), nick("nick"), real_name("Unknown"), user_name("user") {

}