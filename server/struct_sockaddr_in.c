#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	/*This struct is parallel to sockaddr struct but only used for IPv4*/
	struct sockaddr_in test;

	test.sin_family = AF_INET;
	test.sin_port = htons(6667);
	memset(test.sin_zero, 0, sizeof(test.sin_zero));
	test.sin_addr.s_addr = inet_addr("10.11.1.13");
	if (test.sin_addr.s_addr == INADDR_NONE)
	{
		fprintf(stderr, "BAD IP");
		exit(1);
	}
	printf("My IP address in Network Byte Order: %d\n", test.sin_addr.s_addr);
	printf("Check above: %s\n", inet_ntoa(test.sin_addr));
	return 0;
}