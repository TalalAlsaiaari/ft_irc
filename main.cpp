#include "Server.hpp"

int	main(int ac, char **av)
{
	Server	server;

    if (ac == 3) {
        server.setPort(av[1]);
        server.parser.setPass(av[2]);
    } else {
        std::cout << "expected params: <port> <password>\n";
        return 1;
    }
	server.ftIRC();
	return 0;
}
