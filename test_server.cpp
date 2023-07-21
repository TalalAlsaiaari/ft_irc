#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include "Parser.hpp"
#include "Client.hpp"
#include <map>

struct addrinfo	*ft_getaddrinfo(std::string port)
{
	struct addrinfo hints;
	struct addrinfo	*res;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_TCP;
	if ((getaddrinfo(NULL, &port[0], &hints, &res)) < 0)
	{
		printf("getaddrinfo: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	return (res);
}

int	ft_socket(struct addrinfo *res)
{
	int	sockfd;

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0)
	{
		printf("socket: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	// freeaddrinfo(res);
	return (sockfd);
}

void	ft_setsockopt(int sockfd)
{
	int	yes;

	yes = 1;
	if ((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))) < 0)
	{
		printf("setsockopt: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	return ;
}

void	ft_bind(int sockfd, struct addrinfo *res)
{
	if ((bind(sockfd, res->ai_addr, res->ai_addrlen)) < 0)
	{
		printf("bind: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	freeaddrinfo(res);
	return ;
}

void	ft_listen(int sockfd)
{
	if ((listen(sockfd, 10)) < 0)
	{
		printf("listen: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	return ;
}

int	ft_new_client(int sockfd)
{
	struct sockaddr_storage	new_client_addr;
	socklen_t				new_client_addr_size;
	int						new_client;

	new_client_addr_size = sizeof(new_client_addr);
	new_client = accept(sockfd, (struct sockaddr *)&new_client_addr,
		&new_client_addr_size);
	if (new_client < 0)
	{
		printf("accept: %s. Exiting\n", strerror(errno));
		exit(1);
	}
	return (new_client);
}

void	add_to_pfds(struct pollfd **pfds, int new_client, int *fd_count, int *fd_size)
{
	if (*fd_count == *fd_size)
	{
		*fd_size += 1;
		*pfds = (struct pollfd *)realloc(*pfds, sizeof(**pfds) * (*fd_size));
		if (!*pfds)
		{
			printf("realloc: %s. Exiting\n", strerror(errno));
			exit(1);
		}
	}
	(*pfds)[*fd_count].fd = new_client;
	(*pfds)[*fd_count].events = POLLIN;
	*fd_count += 1;
	return ;
}

void	del_from_pfds(struct pollfd *pfds, int i, int *fd_count)
{
	pfds[i] = pfds[*fd_count - 1];
	(*fd_count)--;
	return ;
}

int	main(int ac, char **av)
{
	struct addrinfo	*res;
	int				sockfd;
	int				new_client;
	struct pollfd	*pfds;
	int				fd_size;
	int				fd_count;
	int				poll_count;
	int				nbytes;
	int				sender_fd;
	char			buf[256];
	// int				dest_fd;
    std::map<int, Client> clients;
    Parser parser;
    std::string port;

    if (ac == 3) {
        port = av[1];
        // pass = av[2];
        parser.setPass(av[2]);
    } else {
        std::cout << "expected params: <port> <password>\n";
        return 1;
    }
	res = ft_getaddrinfo(port);
	sockfd = ft_socket(res);
	ft_setsockopt(sockfd);
	ft_bind(sockfd, res);
	ft_listen(sockfd);
	fd_size = 5;
	pfds = (struct pollfd *)malloc(fd_size * sizeof(pfds));
	pfds[0].fd = sockfd;
	pfds[0].events = POLLIN;
	fd_count = 1;
	while (1)
	{
		poll_count = poll(pfds, fd_count, -1);
		if (poll_count == -1)
		{
			printf("poll: %s. Exiting\n", strerror(errno));
			exit(1);
		}
		for (int i = 0; i < fd_count; i++)
		{
			if (pfds[i].revents & POLLIN)
			{
				if (pfds[i].fd == sockfd)
				{
					new_client = ft_new_client(sockfd);
                    clients[new_client] = Client(new_client);
					add_to_pfds(&pfds, new_client, &fd_count, &fd_size);
				}
				else
				{
					nbytes = recv(pfds[i].fd, buf, sizeof(buf), 0);
					sender_fd = pfds[i].fd;
					if (nbytes <= 0)
					{
						if (nbytes == 0)
							printf("clinet %d hung up\n", sender_fd);
						else
							perror("recv");
						close(pfds[i].fd);
						del_from_pfds(pfds, i, &fd_count);
					}
					else
					{
                        parser.takeInput(buf, sender_fd, clients[sender_fd]);
						// for (int j = 0; j < fd_count; j++)
						// {
						// 	dest_fd = pfds[j].fd;
						// 	if (dest_fd != sockfd && dest_fd != sender_fd)
						// 		if (send(dest_fd, buf, nbytes, 0) < 0)
						// 			perror("send");
						// }
					}
				}
			}
		}
	}
	return 0;
}

// /*
// ** pollserver.c -- a cheezy multiperson chat server
// */

// // 001    RPL_WELCOME
//     //    "Welcome to the Internet Relay Network
//     //  @return   <nick>!<user>@<host>"
// // #define RPL_WELCOME 001

// // // 332     RPL_TOPIC
// // #define RPL_TOPIC 332

// //     //   376     RPL_ENDOFMOTD
// //     //                     ":End of /MOTD command"
// // #define RPL_ENDOFMOTD 376


// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <netdb.h>
// #include <poll.h>
// #include <fcntl.h>
// #include "Client.hpp"
// #include "Parser.hpp"

// // #define PORT "9034"   // Port we're listening on

// // Get sockaddr, IPv4 or IPv6:
// void *get_in_addr(struct sockaddr *sa)
// {
//     if (sa->sa_family == AF_INET) {
//         return &(((struct sockaddr_in*)sa)->sin_addr);
//     }

//     return &(((struct sockaddr_in6*)sa)->sin6_addr);
// }

// // Return a listening socket
// int get_listener_socket(std::string port)
// {
//     int listener;     // Listening socket descriptor
//     int yes=1;        // For setsockopt() SO_REUSEADDR, below
//     int rv;

//     struct addrinfo hints, *ai, *p;

//     // Get us a socket and bind it
//     memset(&hints, 0, sizeof hints);
//     hints.ai_family = AF_INET;
//     hints.ai_socktype = SOCK_STREAM;
//     hints.ai_flags = AI_PASSIVE;
//     if ((rv = getaddrinfo(NULL, &port[0], &hints, &ai)) != 0) {
//         fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
//         exit(1);
//     }
//     for(p = ai; p != NULL; p = p->ai_next) {
//         listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
//         if (listener < 0) {
//             continue;
//         }

//         // Lose the pesky "address already in use" error message
//         setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

//         if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
//             close(listener);
//             continue;
//         }

//         break;
//     }

//     // If we got here, it means we didn't get bound
//     if (p == NULL) {
//         return -1;
//     }

//     freeaddrinfo(ai); // All done with this

//     // Listen
//     if (listen(listener, 10) == -1) {
//         return -1;
//     }

//     return listener;
// }

// // Add a new file descriptor to the set
// void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size)
// {
//     // If we don't have room, add more space in the pfds array
//     if (*fd_count == *fd_size) {
//         *fd_size *= 2; // Double it

//         *pfds = (pollfd *)realloc(*pfds, sizeof(**pfds) * (*fd_size));
//     }

//     (*pfds)[*fd_count].fd = newfd;
//     (*pfds)[*fd_count].events = POLLIN; // Check ready-to-read

//     (*fd_count)++;
// }

// // Remove an index from the set
// void del_from_pfds(struct pollfd pfds[], int i, int *fd_count)
// {
//     // Copy the one from the end over this one
//     pfds[i] = pfds[*fd_count-1];

//     (*fd_count)--;
// }
// #include <map>
// // Main
// int main(int ac, char **av)
// {
//     int listener;     // Listening socket descriptor
//     int newfd;        // Newly accept()ed socket descriptor
//     struct sockaddr_storage remoteaddr; // Client address
//     socklen_t addrlen;
//     char buf[256];    // Buffer for client data
//     char remoteIP[INET_ADDRSTRLEN];
//     std::map<int, Client> clients;
//     Parser parse;
//     // std::string pass;
//     std::string port;
//     // Start off with room for 5 connections
//     // (We'll realloc as necessary)

//     if (ac == 3) {
//         port = av[1];
//         // pass = av[2];
//         parse.setPass(av[2]);
//     } else {
//         std::cout << "expected params: <port> <password>\n";
//         return 1;
//     }
//     int fd_count = 0;
//     int fd_size = 5;
//     struct pollfd *pfds = (pollfd *)malloc(sizeof *pfds * fd_size);

//     // Set up and get a listening socket
//     listener = get_listener_socket(port);

//     if (listener == -1) {
//         fprintf(stderr, "error getting listening socket\n");
//         exit(1);
//     }

//     // Add the listener to set
//     pfds[0].fd = listener;
//     pfds[0].events = POLLIN; // Report ready to read on incoming connection

//     fd_count = 1; // For the listener

//     // Main loop
//     for(;;) {
//         int poll_count = poll(pfds, fd_count, -1);
//         // printf("polled\n");
//         if (poll_count == -1) {
//             perror("poll");
//             exit(1);
//         }

//         // Run through the existing connections looking for data to read
//         for(int i = 0; i < fd_count; i++) {

//             // Check if someone's ready to read
//             if (pfds[i].revents & POLLIN) { // We got one!!

//                 if (pfds[i].fd == listener) {
//                     // If listener is ready to read, handle new connection

//                     addrlen = sizeof remoteaddr;
//                     newfd = accept(listener,
//                         (struct sockaddr *)&remoteaddr,
//                         &addrlen);
// 					fcntl(newfd, F_SETFL, O_NONBLOCK);
//                     if (newfd == -1) {
//                         perror("accept");
//                     } else {
//                         add_to_pfds(&pfds, newfd, &fd_count, &fd_size);
//                         printf("pollserver: new connection from %s on ""socket %d\n",inet_ntop(remoteaddr.ss_family,get_in_addr((struct sockaddr*)&remoteaddr),remoteIP, INET_ADDRSTRLEN), newfd);
//                         std::cout << std::string(inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*)&remoteaddr), remoteIP, INET_ADDRSTRLEN)) << std::endl;
//                         clients[newfd] = Client(newfd);
//                     }
//                 } else {
//                     // If not the listener, we're just a regular client
//                     int nbytes = recv(pfds[i].fd, buf, sizeof buf, 0);

//                     int sender_fd = pfds[i].fd;
// 					// printf("%s\n", buf);
//                     // send()
//                     if (nbytes <= 0) {
//                         // Got error or connection closed by client
//                         if (nbytes == 0) {
//                             // Connection closed
//                             printf("pollserver: socket %d hung up\n", sender_fd);
//                         } else {
//                             perror("recv");
//                         }
// 						printf("bye\n");
//                         close(pfds[i].fd); // Bye!

//                         del_from_pfds(pfds, i, &fd_count);

//                     } else {
//                         // We got some good data from a client
//                         printf("%s\n", buf);
//                         parse.takeInput(buf, sender_fd, clients[sender_fd]);
//                         memset(buf, 0, strlen(buf));
//                     }
//                 } // END handle data from client
//             } // END got ready-to-read from poll()
//         } // END looping through file descriptors
//     } // END for(;;)--and you thought it would never end!

//     return 0;
// }
