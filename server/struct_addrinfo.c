#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

int main()
{
	struct addrinfo test;

	test.ai_flags = AI_PASSIVE; // assign local address to socket struct
	test.ai_family = AF_INET; // IPv4
	test.ai_socktype = SOCK_STREAM; // using TCP
	test.ai_protocol = IPPROTO_TCP; // 0 for any protocol, IPROTO_TCP for TCP
	test.ai_addrlen = 0; // on input must be 0
	test.ai_canonname = 0; // on input must be 0
	test.ai_addr = 0; // on input must be 0
	test.ai_next = 0; // on input must be 0

	/*When passing these values to getaddrinfo, it will list all IP addresses and ports
	that are compatible with these limits for the host and service in a linked list of
	the same struct type. The returned values can then be used in later functions,
	such as socket, bind, etc.*/
	
	return 0;
}