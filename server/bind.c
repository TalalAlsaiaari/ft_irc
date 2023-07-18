#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main()
{
	struct addrinfo hints, *res;
	int sockfd;
	// first, load up address structs with getaddrinfo():
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // use IPv4 or IPv6, whichever
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // fill in my IP for me

	getaddrinfo(NULL, "3490", &hints, &res);

	// make a socket:

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	// bind it to the port we passed in to getaddrinfo():

	bind(sockfd, res->ai_addr, res->ai_addrlen);
	return 0;
}

/*By using the AI_PASSIVE flag, I’m telling the program to bind to the IP of the host it’s running on. If
you want to bind to a specific local IP address, drop the AI_PASSIVE and put an IP address in for the first
argument to getaddrinfo().*/