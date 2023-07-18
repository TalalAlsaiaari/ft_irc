#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main()
{
	int s;
	struct addrinfo hints, *res;
	// do the lookup
	// [pretend we already filled out the "hints" struct]
	getaddrinfo("www.example.com", "http", &hints, &res);
	// again, you should do error-checking on getaddrinfo(), and walk
	// the "res" linked list looking for valid entries instead of just
	// assuming the first one is good (like many of these examples do).
	// See the section on client/server for real examples.

	s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	return 0;
}