#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int status;
	struct addrinfo hints;
	struct addrinfo *test;
	struct addrinfo *servinfo; // will point to the results

	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_flags = AI_PASSIVE;	 // fill in my IP for me
	hints.ai_family = AF_INET;	 // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_protocol = IPPROTO_TCP; // for TCP

	printf("\n~~~~~~~~~~~~~~~~~~~~ hints info ~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("ai_flags: AI_PASSIVE = %d\n", hints.ai_flags);
	printf("ai_family: AF_INET = %d\n", hints.ai_family);
	printf("ai_socktype: SOCK_STREAM = %d\n", hints.ai_socktype);
	printf("ai_protocol: TCP = %d\n", hints.ai_protocol);
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	/* getaddrinfo is used to get a list of IP addresses and port numbers
	for host (first param) and service (second param)*/
	if ((status = getaddrinfo(NULL, "irc", &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}
	int i = 1;
	test = servinfo;
	while (test)
	{
		printf("\n~~~~~~~~~~~~~~~~~~~~ serv info ~~~~~~~~~~~~~~~~~~~~~~\n");
		printf("ai_flags[%d]: %d\n",i, test->ai_flags);
		printf("ai_family[%d]: %d\n",i, test->ai_family);
		printf("ai_socktype[%d]: %d\n",i, test->ai_socktype);
		printf("ai_protocol[%d]: %d\n",i, test->ai_protocol);
		printf("ai_addrlen[%d]: %u\n",i, test->ai_addrlen);
		printf("ai_canonname[%d]: %s\n",i, test->ai_canonname);
		printf("ai_ai_addr->sa_family[%d]: %hhu\n",i, test->ai_addr->sa_family);
		printf("ai_ai_addr->sa_data[%d]: %s\n",i, test->ai_addr->sa_data);
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		test = test->ai_next;
		i++;
	}

	// ... do everything until you don't need servinfo anymore ....

	freeaddrinfo(servinfo); // free the linked-list
	return 0;
}