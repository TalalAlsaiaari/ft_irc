#include "Server.hpp"

int	main(int ac, char **av)
{
	Server	server;

    try
	{
        if (ac == 3) {
            server.setPort(av[1]);
            server.parser.setPass(av[2]);
        } else {
            std::cout << "expected params: <port> <password>\n";
            return 1;
        }
            server.ftIRC();
        }
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
	return 0;
}
